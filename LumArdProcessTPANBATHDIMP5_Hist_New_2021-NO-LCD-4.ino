//Se definen variables
//Histéresis para panel
float Histp;
//Histéresis para batería
float Histb;
//Voltaje de Panel
float Vp;
//Voltaje de Batería
float Vb;
//Voltaje de Panel superior
float Vlpu;
//Voltaje de Panel inferior
float Vlpd;
//Voltaje de Batería superior
float Vlbu;
//Voltaje de Batería inferior
float Vlbd;
//Voltaje medio de Panel
float Vnp;
//Voltaje medio de Batería
float Vnb;
//Entradas analógicas 
float a0;
float a1;
//Se definen contadores
int i; 
long count;
// Se define una variable para el estado de la conmutación
int state;

void setup() {
  // put your setup code here, to run once:
//Valores de inicialización para cada una de las variables
Vp = 0.0;
Vb = 0.0;
Histp = 1.0;
Histb = 0.5;
Vnp = 15.0;
Vnb = 12.0;
Vlpu = Vnp + Histp;
Vlpd = Vnp - Histp;
Vlbu = Vnb + Histb;
Vlbd = Vnb - Histb;
count = 0;
a0 = 0.0;
a1 = 0.0;
//Establecer roles de pines y configurar a nivel bajo por defecto
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
digitalWrite(6, LOW);
digitalWrite(7, LOW);
//Estado de conmutación será 0 (Usa Red Eléctrica)
state = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  //Lectura de voltaje de panel
  a0 = analogRead(0);
  Vp = (a0*20.0)/1024.0;
  //Lectura de voltaje de batería
  a1 = analogRead(1);
  Vb = (a1*20.0)/1024.0;
  //Si Voltaje de panel es mayor a voltaje superior de panel "Y"
  //Voltaje de batería es mayor a voltaje superior de batería
  //"O"
  //Voltaje de panel es menor a voltaje inferior de panel "Y"
  //Voltaje de batería es mayor a voltaje superior de batería
  //if ((Vp>Vlpu && Vb>Vlbu) || (Vp<Vlpd && Vb>Vlbu)){
  if ((Vp>Vlpu && Vb>Vlbu) || (Vp<Vlpd && Vp>Vb && Vb>Vlbu)){
    //Si Estado de conmutación es 0 (Usa Red Eléctrica)
    if (state==0){
      //Cambiar estado de conmutación a 1 (Usa Batería)
      digitalWrite(7, HIGH);
      //Esperar unos segundos
      delay(2500);
      //Conectar batería a la carga o salida
      digitalWrite(6, HIGH);
      //Notificar estado de conmutación 1 (Usa Batería)
      state = 1;
    }
    delay(1);
    //Si el contador es menor de 10 millones
    //aumentar el contador en 1
    if (count<10000000){
      count+=1;
    }
  //Si Voltaje de panel es menor a voltaje inferior de panel "Y"
  //Voltaje de batería es menor a voltaje inferio de batería
  //"O"
  //Voltaje de panel es mayor a voltaje superior de panel "Y"
  //Voltaje de batería es menor a voltaje inferio de batería
  }else if ((Vp<Vlpd && Vb<Vlbd) || (Vp>Vlpu && Vb<Vlbd)){
    //Cambiar estado de conmutación a 0 (Usa Red Eléctrica)
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    //Notificar estado de conmutación 0 (Usa Red Eléctrica)
    state = 0;
    //Si el contador es menor de 600mil entonces esperar 1200 segundos
    //(20 minutos de carga de batería)
    if (count<600000){
      delay(1200000);
    //Si el contador es mayor de 600mil y menor 6 millones entonces esperar 600 segundos
    //(10 minutos de carga de batería)
    }else if (count>=600000 && count<6000000){
      delay(600000);
    //Si el contador es mayor de 6 millones entonces esperar 300 segundos
    //(5 minutos de carga de batería)
    }else{
      delay(300000);
    }
    //Establecer contador a 0
    count = 0;
  }else{
  //Si los voltajes están dentro de los valores que soporta la histéresis
  //No se toma ninguna acción
  }
}
