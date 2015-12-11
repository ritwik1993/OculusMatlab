close all

g = GantryMove('192.168.1.121');
g.Connect;

clearvars -except g

t = tcpclient('localhost', 1700);
%use localhost when the server is on same computer
% otherwise use IP adddress of the server
i = 0;
k = 1;


g.setESPodFreqPhase(2000,0);
prev = 0;
prevPos = [0, 0, 0, 0];

while (true)
    if t.BytesAvailable
        data1 = t.read;
        data1 = char(data1);
        yawData = strread(data1, '%f', 'delimiter', ';');
        yaw = yawData(end - 2);
        X = ['The current yaw is: ', num2str(yaw)];
        currPos = g.GetGanPosSnap;
        if any(isempty(currPos))
            currPos = prevPos;
        end
        prevPos = currPos;
        %g.goToPos([currPos(1), currPos(2), currPos(3), currPos(4)]);
        g.goToPos([currPos(1), currPos(2), currPos(3), yaw*pi/180]);
        disp(X);
        es = g.GetGanESSnap;
        if any(isnan(es))
            t.write([uint8(num2str(currPos(4)*180/pi)) 44  uint8(num2str(currPos(1)*100)) 44 uint8(num2str(prev)) 44 uint8(num2str(-currPos(2)*100)) 13]);
        else
            es = abs(es);
            if es(5) > 1
                es(5) = 1;
            end
            %es = log(es);
            esN = ((es(5) - ( 0)) / (1)-(0)) * 100;
            prev = esN;
            t.write([uint8(num2str(currPos(4)*180/pi)) 44  uint8(num2str(currPos(1)*100)) 44 uint8(num2str(esN)) 44 uint8(num2str(-currPos(2)*100)) 13]);
        end
%         t.write([uint8(num2str(yaw)) 44  uint8(num2str(0)) 44 uint8(num2str(i)) 44 uint8(num2str(0)) 13]);
%         i = i+k;
%         if i == 100
%             k = -1;
%         end
%         if i == 0
%             k = 1;
%        end
    end
end
