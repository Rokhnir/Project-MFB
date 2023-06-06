void dda(){
    while(){
        currentX += pasX;
        currentY += pasY;

        int currentMapX = floor(currentX/(screenWidth/mapWidth));
        int currentMapY = floor(currentY/(screenHeight/mapHeight));
        
        if(map[mapHeight-currentMapY-1][currentMapY]) hit = 1;
    }
}