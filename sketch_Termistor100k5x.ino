/*Este programa coleta 15 dados de temperatura a cada 30 minutos com o objetivo de identificar quando o
sistema estudado entra em estado estácionário a partir da diferença entre dois conjuntos de 15 dados.*/
/*Os dados dos pinos A0 - A4 são lidos, convertidos para dados de tensão Vout[0] - Vout[4], então
sabendo que os resistores conectados aos sensores são de 100kΩ, calcula-se a resistência Rout[0]-Rout[4] 
dos termistores que por sua vez, são convertidos para valores de temperatura TKelvin[0]-TKelvin[4]
e TCelsius[0] - TCelsius[4]. Estes são mandados para a planilha eletrônica*/
#define Nterm 5 // Número de termistores utilizados
#define Term0 A0
#define Term1 A1
#define Term2 A2
#define Term3 A3
#define Term4 A4
#define Vin 4.85
#define T0 298.15
#define Rt 100000.0
#define R0 100000.0
float beta[Nterm];
float Rinfty[Nterm];
float TKelvin[Nterm];
float TCelsius[Nterm];
float Vout[Nterm];
float Rout[Nterm];
int Ndados = 0;//contador para controlar a quantidade de dados coletados
/*A planilha eletrônica é controlada pelo código Arduino através da função Serial.println*/
void setup() {
  for(int i=0;i<Nterm;i++){
    beta[i]=3950.0;
    Rinfty[i]=R0*exp(-beta[i]/T0);
  }
  Serial.begin(9600);
  delay(100);
  Serial.println("CLEARSHEET");//Limpa a planilha de dados
  delay(100);
  Serial.println("CLEARDATA");
  delay(100);
  Serial.println("LABEL,Tempo,T0,T1,T2,T3,T4");//insere um cabeçalho na planilha de dados
  delay(1800000); //espera 30 min antes de coletar os primeiros dados
}

void loop() {
 while (Ndados<15){ //coleta 15 dados de temperatura com espaçamento de 1 segundo entre eles
 Vout[0] = Vin*((float)(analogRead(Term0))/1023.0);
 Vout[1] = Vin*((float)(analogRead(Term1))/1023.0);
 Vout[2] = Vin*((float)(analogRead(Term2))/1023.0);
 Vout[3] = Vin*((float)(analogRead(Term3))/1023.0);
 Vout[4] = Vin*((float)(analogRead(Term4))/1023.0);
 for(int n=0;n<Nterm;n++){
 Rout[n] = Rt*Vout[n]/(Vin-Vout[n]);
 TKelvin[n] = beta[n]/log(Rout[n]/Rinfty[n]);
 TCelsius[n]=TKelvin[n]-273.15;
 }
 Serial.println((String)"DATA,TIMER,"+ TCelsius[0] + "," + TCelsius[1] + "," + TCelsius[2] + "," + TCelsius[3] + "," + TCelsius[4] + ",AUTOSCROLL_20");//envia os dados para o PLX-DAQ
 delay(1000);
 Ndados += 1;
 }
 Serial.println("SAVEWORKBOOKAS,Log100k5x");//salva a planilha a cada 15 dados de temperatura
 delay(1800000);//espera mais 30 min antes de coletar dados novamente
 Ndados = 0;//reinicia o contador para coletar mais 15 dados
}
