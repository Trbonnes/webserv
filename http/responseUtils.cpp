// #include "HttpResponse.hpp"

// inline bool   HttpResponse::is_base64(unsigned char c)
// {
//   return (isalnum(c) || (c == '+') || (c == '/'));
// }

// //** Encode password in base64 **
// std::string   HttpResponseOld::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
// {
//   std::string   ret;
//   int           i;
//   int           j;
//   unsigned char char_array_3[3];
//   unsigned char char_array_4[4];

//   i = 0;
//   j = 0;
//   while (in_len--)
//   {
//     char_array_3[i++] = *(bytes_to_encode++);
//     if (i == 3)
//     {
//       char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
//       char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
//       char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
//       char_array_4[3] = char_array_3[2] & 0x3f;
//       for(i = 0; (i <4) ; i++)
//         ret += _base64_chars[char_array_4[i]];
//       i = 0;
//     }
//   }
//   if (i)
//   {
//     for(j = i; j < 3; j++)
//         char_array_3[j] = '\0';
//     char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
//     char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
//     char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
//     char_array_4[3] = char_array_3[2] & 0x3f;
//     for (j = 0; (j < i + 1); j++)
//         ret += _base64_chars[char_array_4[j]];

//     while((i++ < 3))
//         ret += '=';
//   }
//   return ret;
// }

// //** Decode password in base64 **
// std::string   HttpResponse::base64_decode(std::string const& encoded_string) {
//     int           in_len;
//     int           i;
//     int           j;
//     int           in_;
//     unsigned char char_array_4[4];
//     unsigned char char_array_3[3];
//     std::string   ret;

//     in_len = encoded_string.size();
//     i = 0;
//     j = 0;
//     in_ = 0;
//     while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
//     {
//         char_array_4[i++] = encoded_string[in_]; in_++;
//         if (i ==4) {
//         for (i = 0; i <4; i++)
//             char_array_4[i] = _base64_chars.find(char_array_4[i]);
//         char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
//         char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
//         char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
//         for (i = 0; (i < 3); i++)
//             ret += char_array_3[i];
//         i = 0;
//         }
//     }
//     if (i) {
//         for (j = i; j <4; j++)
//         char_array_4[j] = 0;
//         for (j = 0; j <4; j++)
//         char_array_4[j] = _base64_chars.find(char_array_4[j]);
//         char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
//         char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
//         char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
//         for (j = 0; (j < i - 1); j++)
//         ret += char_array_3[j];
//     }
//     return ret;
// }

// std::string     HttpResponse::acceptLanguage()
// {
//     std::vector<std::string>::iterator itClientBegin;
//     std::vector<std::string>::iterator itClientEnd;
//     std::vector<std::string>::iterator itServer;
//     std::vector<std::string>::iterator itServerEnd;
//     std::string str;
//     std::string trydir;
//     struct stat dir;
//     int r;

//     if (!_config.getLanguage(_location).empty())
//     {
//         str.assign("/");
//         itClientBegin = _request->getAcceptLanguage().begin();
//         itClientEnd = _request->getAcceptLanguage().end();
//         itServerEnd = _config.getLanguage(_location).end();
//         while (itClientBegin != itClientEnd)
//         {
//             itServer = std::find(_config.getLanguage(_location).begin(), _config.getLanguage(_location).end(), *itClientBegin);
//             if (itServer != itServerEnd)
//             {
//                 _contentLanguage = *itServer;
//                 str.assign(*itServer);
//                 str.append("/");
//                 r = stat(trydir.assign(_route).append(str).c_str(), &dir);
//                 if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
//                     return (str);
//                 else
//                     _contentLanguage.assign("");
//             }
//             itClientBegin++;
//         }
//         _contentLanguage = *(_config.getLanguage(_location).begin());
//         str.append(*(_config.getLanguage(_uri).begin()));
//         r = stat(trydir.assign(_route).append(str).c_str(), &dir);
//         if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
//             return (str);
//         else
//             return (_contentLanguage.assign(""));
//     }
//     return ("");
// }