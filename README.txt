Dupla: Anderson Leal e Caio Freitas

Para a realiza��o do trabalho, n�s usamos como comunica��o o aplicativo WhatsApp
e para compartilhamento dos c�digos, o DropBox, visto que n�o tinhamos muita experi�ncia
com o GitHub. N�o nos encontramos pessoalmente, mas o trabalho foi perfeitamente realizado.
Anderson ficou respons�vel pelo server e eu, Caio, fiquei respons�vel pelo client, mas em
todo tempo ajudamos um ao outro.
Nos c�digos, para a conex�o do client, � pedido no in�cio que o usu�rio digite o IP e a porta
do server e ainda um nickname desejado para o chat. Como protocolo implementamos que, para
mensagens broadcast, basta o usu�rio digitar uma mensagem que ela ser� redirecionada para todos.
Para mensagens privadas, o usu�rio deve digitar # e o nickname do destinat�rio e a mensagem.
Assim ela ser� redirecionada apenas para o destinat�rio desejado. Exemplo: #caio Teste
Ainda, para consultar os nicknames conectados ao chat, basta o usu�rio digitar !
Para a leitura n�o blocante do client, utilizamos as fun��es kbhit e getch da biblioteca conio.h
e para o restante dos c�digos, tomamos como base os c�digos estudados em aula.