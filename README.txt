Dupla: Anderson Leal e Caio Freitas

Para a realização do trabalho, nós usamos como comunicação o aplicativo WhatsApp
e para compartilhamento dos códigos, o DropBox, visto que não tinhamos muita experiência
com o GitHub. Não nos encontramos pessoalmente, mas o trabalho foi perfeitamente realizado.
Anderson ficou responsável pelo server e eu, Caio, fiquei responsável pelo client, mas em
todo tempo ajudamos um ao outro.
Nos códigos, para a conexão do client, é pedido no início que o usuário digite o IP e a porta
do server e ainda um nickname desejado para o chat. Como protocolo implementamos que, para
mensagens broadcast, basta o usuário digitar uma mensagem que ela será redirecionada para todos.
Para mensagens privadas, o usuário deve digitar # e o nickname do destinatário e a mensagem.
Assim ela será redirecionada apenas para o destinatário desejado. Exemplo: #caio Teste
Ainda, para consultar os nicknames conectados ao chat, basta o usuário digitar !
Para a leitura não blocante do client, utilizamos as funções kbhit e getch da biblioteca conio.h
e para o restante dos códigos, tomamos como base os códigos estudados em aula.