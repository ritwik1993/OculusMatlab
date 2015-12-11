close all
clear all
g = GantryMove('192.168.1.121');
g.Connect;
g.setESPodFreqPhase(2000,0);
endpos = [0.4,-0.15,-0.2,0];
startpos = [-0.4,-0.15,-0.2,0];
g.goToPos(startpos);
pause(10)

t = tcpclient('192.168.1.124', 1700);

g.DemoLinTrajOculus(startpos, endpos, t);


