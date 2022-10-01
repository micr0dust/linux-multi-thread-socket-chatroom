# linux-multi-thread-socket-chatroom

multi-thread socket chatroom in Linux

執行結果與說明：
此程式只能在 Linux 環境下運作。
server 在還沒有跟 client 互動前不會有輸出。
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a1.png?raw=true)

而 client 在連線後會被分配到其他port，並收到server傳來的歡迎訊息
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a2.png?raw=true)

client 如果輸入訊息就會送到 server，server 會回傳匿名和該訊息給當下所有的連線
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a3.png?raw=true)

而伺服器端則在終端機輸出該連線的ip、port和其以發送的訊息
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a4.png?raw=true)

這時如果有其他client連線到server，他能接收到之後的訊息，但不會有之前的訊息。
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a5.png?raw=true)

因為訊息會全socket同步，所以能看到其他人的訊息
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a6.png?raw=true)

如果有client離線，socket會輸出離線訊息
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a7.png?raw=true)

如果是server端主動關閉，client端會輸出server關閉的訊息
![img](https://github.com/micr0dust/linux-multi-thread-socket-chatroom/blob/main/img-for-md/a8.png?raw=true)
