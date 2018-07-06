int heartCount = 0;

bool isHeartbeat(){
  heartCount++;
  if (heartCount > HEART_LIMIT){
    heartCount=0;
    return true;
  }
  return false;
}

