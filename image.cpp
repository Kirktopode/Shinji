Image::Image(){

  rgb = new int[1][1][3];
  
  for(int i = 0; i < 3; i++){
    
  }

}

Image::Image(int Lrows, int Lcolumns, int*** Lrgb): rows(Lrows), columns(Lcolumns){

  rgb = new int[rows][columns][3];
  
  for(int i = 0; i < rows; i++){
    for(int n = 0; n < columns; n++){
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
  
  rgb = new int[Limage.rows][Limage.columns][3];
  rows = Limage.rows;
  columns = Limage.columns;
  
  for(int i = 0; i < rows; i++){
    for(int n = 0; n < columns; n++){
      for(int t = 0; t < 3; t++){
        rgb[i][n][t] = Limage.rgb[i][n][t];
      }
    }
  }
}

void Image::operator=(const Image &Limage){

  delete [] rgb;
  
  rgb = new int[Limage.rows][Limage.columns][3];
  rows = Limage.rows;
  columns = Limage.columns;
  
  for(int i = 0; i < rows; i++){
    for(int n = 0; n < columns; n++){
      for(int t = 0; t < 3; t++){
        rgb[i][n][t] = Limage.rgb[i][n][t];
      }
    }
  }

}

void Image::SetImageInfo(int Lrows, int Lcolumns, int*** Lrgb){
  
}

void Image::getImageInfo(int &numRows, int &numCols, int*** Lrgb){

}

int* Image::getPixelVal(){

}

void Image::setPixelVal(int row, int col, int r, int g, int b){

}

bool Image::inBounds(int row, int col){

}

void Image::getSubImage(int upperLeftRow, int upperLeftCol, int lowerRightRow, 
                 int lowerRightCol, Image& LImage)

void Image::enlargeImage(int value, Image& LImage)

void Image::shrinkImage(int value, Image& LImage)

void Image::reflectImage(bool flag, Image& LImage)

void Image::translateImage(int value, Image& LImage)

void Image::rotateImage(int value, Image& LImate)

Image Image::operator+(const Image &LImage)

Image Image::operator-(const Image &LImage)

void Image::negateImage(Image& LImage)
