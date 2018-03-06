void asama2() {

bool sagserit = false;
bool solserit = false;

int seritsayacı = 0;

while(position != 7000){
  
// sağ ve sol şerit boolean'ları true ise o taraftakli şeritler boş demektir, false ise dolu demektir.
// önünde cisim varsa boş şeride geçicek (if'le falan). Geçtikten sonra şerit sayacı geçilen şeride göre arttırılacak veya azaltılacak(sağa geçerse +, sola geçerse -).
// Şerit değiştirme if'inin altına sayacın -2 veya +2 olma durumu da eklenmelidir. Bu durumda araç en kenar şeritlerde demektir. Bu durumda da şerit boolean'larının durumlarına göre ya direk yan  
// şeride geçicek ya da yan şeritler doluysa geri gidecek...

if(sharp2 > 400){      // sharp'lar için girile sınır değerler rastgeledir şu anda.
  solserit = true;
  }
else{
  solserit = false;
  }


if(sharp4 > 400){
  sagserit = true;
  }
else{
  solserit = false;
  }


switch (solserit){
  
  case true:
  if(seritsayacı > 0){
  //sol şeride  geçme kodu  
  }
  break;
  
  default
  break;
  }            


switch (sagserit){
  
  case true:
  if(seritsayacı < 0){
    // sağ şeride geçme kodu
    }
  break;
  
  default
  break;
  }
      
    
    
 }
}
