#ifndef NETWORK_H
#define NETWORK_H

//An interesting anti-hacking mechanic would be NOT banning players but instead hack-listing them and putting them in matches with believable usernames and AI opponents (alongside anyone in their party).

void OpenNetwork(const char* host);
void WaitNetwork();
int SendThread(void*); //an abstract function that would eventually send 256 bytes over the network in a particular order like 0-3 damageDealt, 4-7 posX, 8-11 posY, 12-15 velX, 16-19 velY, etc
int ReceiveThread(void*); //this function would receive the info, apply damage via health -= damageDealt (rather than setting the health), set the positions + vel*elapsedTime (since timestamp was received), and set the velocity minus any gravity adjustments

//the server will be non-authoritative meaning the enemy could basically set your health to zero if they wanted.
//the enemy could also teleport, etc

//Send your position over the network followed by whether or not you hit the enemy.

//posX,posY,velX,velY,state,damageDealt,knockbackX,knockbackY

#endif