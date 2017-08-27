#include "util.h"

int fetchOpts(int argc, char **args, Opts *opts){
  int opt;

  while ((opt = getopt(argc, args, "td:n:c:")) != -1)
    switch (opt) {
    case 't':
      opts->luksDevPath = DEFAULT_LUKSDEVPATH;
      opts->luksDevName = DEFAULT_LUKSDEVNAME;
      opts->testMode = true;
      break;
    case 'd':
      opts->luksDevPath = optarg;
      break;
    case 'n':
      opts->luksDevName = optarg;
      break;
    case 'c':
      opts->confPath = optarg;
      break;
    default:
      fprintf(stdout, "Usage: osk_mouse [-t] [-d /dev/sda] [-n device_name] [-c /etc/osk.conf]\n");
      return 1;
    }
  if (opts->luksDevPath.empty()) {
    fprintf(stderr, "No device path specified, use -d [path] or -t\n");
    return 1;
  }
  if (opts->luksDevName.empty()) {
    fprintf(stderr, "No device name specified, use -n [name] or -t\n");
    return 1;
  }
  if (opts->confPath.empty()){
    opts->confPath = DEFAULT_CONFPATH;
  }
  return 0;
}


Uint32 time_left(Uint32 now, Uint32 next_time) {
  if (next_time <= now)
    return 0;
  else
    return next_time - now;
}


string strList2str(list<string> strList){
  string str = "";
  list<string>::const_iterator it;
  for (it = strList.begin(); it != strList.end(); ++it){
    str.append(*it);
  }
  return str;
}


SDL_Surface* make_wallpaper(SDL_Renderer *renderer, Config *config,
                            int width, int height){
  SDL_Surface *surface;
  Uint32 rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
  #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
  #endif

  surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask,
                                 bmask, amask);

  if(config->wallpaper[0] == '#'){
    int r, g, b;
    if(sscanf(config->wallpaper.c_str(), "#%02x%02x%02x", &r, &g, &b)!=3){
      fprintf(stderr, "Could not parse color code %s\n", config->wallpaper.c_str());
      exit(1);
    }
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
  }else{
    // Implement image loading
    fprintf(stderr, "Image loading not supported yet\n");
    exit(1);
  }
  return surface;
}