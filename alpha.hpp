const int width    = 800,
          heigth   = 600;


int makeAlpha(const char *front_name, const char *back_name);

int useSSE(sf::Uint32 *front, sf::Uint32 *back, sf::Uint32 *src);
int noSSE (sf::Uint32 *front, sf::Uint32 *back, sf::Uint32 *src);