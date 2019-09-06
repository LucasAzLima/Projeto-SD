import socket
import thread
import time
import serial
from threading import Thread

HOST = '127.0.0.1'   
PORT = 5021    
ArduinoSerial = serial.Serial('COM5',9600, timeout= 10)

def condicoes(gas, umi, temp):
    
    if (temp >= 26 or umi >= 80): 
        ArduinoSerial.write("S1")
        time.sleep(1)
    else:
        ArduinoSerial.write("S2")
        time.sleep(1)

    if (gas >= 300):
        ArduinoSerial.write("S3")
        time.sleep(1)
        
    else:
        ArduinoSerial.write("S4")
        time.sleep(1)
     
    if (temp >= 80 and gas >= 300):
        ArduinoSerial.write("S5")
        time.sleep(1)
  

def conectado(con, cliente):
    print 'Conectado por', cliente

    while True:
        msg = con.recv(1024)
        if not msg: break
        print "cliente diz: ", msg
        ArduinoSerial.write(msg)
        

    print 'Finalizando conexao do cliente', cliente
    con.close()

def dados():
    while True:
        try:
            
            Dado = ArduinoSerial.readline().encode("utf-8")
            dados = str(Dado).split(" ")
            gas = int(dados[0])
            umi = int(dados[1])
            temp = int(dados[2])
            time = int(dados[3])
            print Dado
            print "tempo gasto:",time,"ms"
            condicoes(gas,umi,temp);
        except:
            print "dado vazio"
            
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

orig = (HOST, PORT)

tcp.bind(orig)
tcp.listen(1)

while True:
    con, cliente = tcp.accept()
    Socket = Thread(target=conectado, args=(con, cliente))
    dadosS = Thread(target=dados)
    
    dadosS.start()
    Socket.start()
    Socket.join()
    dadosS.join()

    
tcp.close()
