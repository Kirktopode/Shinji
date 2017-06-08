#ifndef IMAGE_H
#def IMAGE_H

class Image{

  private:
  
    int rows;
    int columns;
    int*** rgb;
  
  public:

    Image();
    Image(int Lrows, int Lcolumns, int*** Lrgb);
    ~Image();
    Image(const Image& LImage);
    void operator=(const Image &Limage);
    void setImageInfo(int Lrows, int Lcolumns, int*** Lrgb);
    void getImageInfo(int &numRows, int &numCols, int*** Lrgb);
    int* getPixelVal(int row, int col);
    void setPixelVal(int row, int col, int r, int g, int b);
    bool inBounds(int row, int col);
    void getSubImage(int upperLeftRow, int upperLeftCol, int lowerRightRow, 
                     int lowerRightCol, Image& LImage);
    void enlargeImage(int value, Image& LImage);
    void shrinkImage(int value, Image& LImage);
    void reflectImage(bool flag, Image& LImage);
    void translateImage(int value, Image& LImage);
    void rotateImage(int value, Image& LImate);
    Image operator+(const Image &LImage);
    Image operator-(const Image &LImage);
    void negateImage(Image& LImage);

};

#endif
