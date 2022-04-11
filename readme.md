# Alpha blending

### This repository was created to combine several images on one using alpha blending

</br>

### You can switch between two image rendering modes. According to the standard, the image is rendered using SSE instructions, if you hold down the "A" key, the SSE instructions will be disabled.

### Installation
    git clone https://github.com/jirol9xa/Alpha-Blending

For running:
    
    make && ./alpha

# Performance research

### My laptop has AMD ryzen 5 4500u installed. Therefore, all performance results are carried out on this processor. The measurement of the number of constructed frames was carried out for 5 seconds

|SSE |noSSE|
|:--:|:---:|
|2446|2093 |

# Conclusion

### Without SSE instructions it has 2093 frames, and with SSE instructions it has 2446. Thus, using SSE instructions gives a 17% performance gain.