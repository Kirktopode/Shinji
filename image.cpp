Image::Image(): rows(1), columns(1) {

  rgb = new int[1][1][3];
  
  for(int i = 0; i < 3; i++){
    rgb[0][0][i] = 0;
  }

}

Image::Image(int Lrows, int Lcolumns, int*** Lrgb): rows(Lrows), columns(Lcolumns){

  rgb = new int**[rows];
  
  for(int i = 0; i < rows; i++){
    rgb[i] = new int*[columns];
    for(int n = 0; n < columns; n++){
      rgb[i][n] = new int[3];
      for(int t = 0; t < 3; t++){
        rgb[i][n][t] = Lrgb[i][n][t];
      }
    }
  }

}

Image::~Image(){

  delete [] rgb;
  
}

Image::Image(const Image &LImage){
  
  rows = Limage.rows;
  columns = Limage.columns;

  rgb = new int**[rows];
  
  for(int i = 0; i < rows; i++){
    rgb[i] = new int*[columns];
    for(int n = 0; n < columns; n++){
      rgb[i][n] = new int[3];
      for(int t = 0; t < 3; t++){
        rgb[i][n][t] = Lrgb[i][n][t];
      }
    }
  }
}

void Image::operator=(const Image &Limage){

  
  for(int i = 0; i < rows; i++){
    for(int n = 0; n < columns; n++){
      delete [] rgb[i][n];
    }
    delete [] rgb[i];
  }
  delete [] rgb;
  
  rows = Limage.rows;
  columns = Limage.columns;
  rgb = new int**[rows];
  
  for(int i = 0; i < rows; i++){
    rgb[i] = new int*[columns];
    for(int n = 0; n < columns; n++){
      rgb[i][n] = new int[3];
      for(int t = 0; t < 3; t++){
        rgb[i][n][t] = Limage.rgb[i][n][t];
      }
    }
  }

}

bool Image::SetImageInfo(int Lrows, int Lcolumns, int*** Lrgb){
  
  if(Lrows > rows || row < 0 || Lcolumns > columns || col < 0){
    return false;
  }
  
  rows = Lrows;
  columns = Lcolumns;
  
  for(int i = 0; i < rows; i++){
    for(int n = 0; n < columns; n++){
      for(int t = 0; t < 3; t++){
        rgb[i][n][t] = Lrgb[i][n][t];
      }
    }
  }
  
  return true;
}

void Image::getImageInfo(int &numRows, int &numCols, int*** Lrgb){
  
  numRows = rows;
  numColumns = columns;
  
  for(int i = 0; i < rows; i++){
    for(int n = 0; n < columns; n++){
      for(int t = 0; t < 3; t++){
        Lrgb[i][n][t] = rgb[i][n][t];
      }
    }
  }

}

bool Image::getPixelVal(int row, int col, int* rgbReturn){
  
  if(row > rows || row < 0 || col > columns || col < 0){
    return false;
  }
  
  for(int t = 0; t < 3; t++){
    rgbReturn[row][col][t] = rgb[row][col][t];
  }

  return true;

}

bool Image::setPixelVal(int row, int col, int r, int g, int b){
  
  if(row > rows || row < 0 || col > columns || col < 0){
    return false;
  }
  
  rgb[row][col][0] = r;
  rgb[row][col][1] = g;
  rgb[row][col][2] = b;

  return true;
}

bool Image::inBounds(int row, int col){
 return !(row > rows || row < 0 || col > columns || col < 0);
}

void Image::getSubImage(int upperLeftRow, int upperLeftCol, int lowerRightRow, 
                 int lowerRightCol, Image& LImage){
                 
}

void Image::enlargeImage(int value, Image& LImage){

}

void Image::shrinkImage(int value, Image& LImage){

}

void Image::reflectImage(bool flag, Image& LImage){

}

void Image::translateImage(int value, Image& LImage){

}

void Image::rotateImage(int value, Image& LImate){

}

Image Image::operator+(const Image &LImage){

}

Image Image::operator-(const Image &LImage){

}

void Image::negateImage(Image& LImage){

}
