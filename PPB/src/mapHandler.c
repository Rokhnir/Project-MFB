void createMap(){
    for(int i = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            switch(map[i][j]){
                case 1:
                    couleurCourante(255,0,0);
                    break;
                case 2:
                    couleurCourante(0,255,0);
                    break;
                case 3:
                    couleurCourante(0,0,255);
                    break;
                case 4:
                    couleurCourante(255,255,255);
                    break;
                default:
                    couleurCourante(0,255,255);
                    break;
            }
            rectangle((screenWidth / mapWidth) * j, (screenHeight / mapHeight) * (screenHeight - i), (screenWidth / mapWidth) * (j + 1), (screenHeight / mapHeight) * (mapHeight - 1 - i));
        }
    }
}