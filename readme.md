# Alpha blending

### This repository was created to combine several images on one using alpha blending

</br>

### You can switch between two image rendering modes. According to the standard, the image is rendered using SSE instructions, if you hold down the "A" key, the SSE instructions will be disabled.

### Installation
    git clone https://github.com/jirol9xa/Alpha-Blending

For running:
    
    make && ./alpha

# Performance research

### My laptop has AMD ryzen 5 4500u installed. Therefore, all performance results are carried out on this processor. Measuring the rendering time of 10 frames in seconds, when processing each 5000 times.Such a large number of constructions were chosen in order to reduce the error introduced by the use of the graphics library by less than 5%.

|SSE    |noSSE -O3|SSE without drawing in SFML|
|:-----:|:-------:|:-------------------------:|
|15.4915|22.7075  |15.4669                    |

# Conclusion

### Using SSE instructions speeds up program execution by 30% (compared to the version without SSE, but with the -O3 flag). The error introduced by the graphics library is less than one percent.