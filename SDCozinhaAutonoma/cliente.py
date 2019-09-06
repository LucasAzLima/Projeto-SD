import socket
import os
import serial
 
HOST = '127.0.0.1' #ip
PORTA = 5021 #porta
 
tcpSOCKET = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
destinoCONEXAO = (HOST, PORTA)
tcpSOCKET.connect(destinoCONEXAO)


 
os.system("clear")
print "|====================================|"
print "|    Arduino na rede usando Python   |"
print "|====================================|"
print "| Digite SAIR para teminar a conexao |"
 
dados = raw_input()
 
while dados != 'sair':
    
    tcpSOCKET.send (dados)
    dados = raw_input()
tcpSOCKET.close()
