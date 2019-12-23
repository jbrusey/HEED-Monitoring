/** @file HeartBeat.ino
 *  @brief Implement the heartbeat function.  
 *
 *  This contains the function to check the time elapsed 
 *  since last packet stored to check if heartbeat packet
 *  should be sent.
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */
 
#include "SUM.h"
int heartCount = 0;

bool isHeartbeat(){
  heartCount++;
  if (heartCount > HEART_LIMIT){
    heartCount=0;
    return true;
  }
  return false;
}
