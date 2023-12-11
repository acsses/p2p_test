# Message
orochi retail some types of messages.

following is the table of these types

|type     |describe|
|---------|----|
|ReqNodeList|request other nodes or discovery srevers to give me Nodelist.|
|ReqNodeAddr|requeset other nodes or discovery srevers to give me designated node's address|
|ReqRecvBlock|requeset other nodes or discovery srevers to give me the oriented block for me|
|ReqProxy|requeset other nodes or discovery srevers to become proxy my accessing to Web server|
|SendOrientedBlock|Send Oriented Block to designated Node|
|SendBroCasBlock|Broadcast Block to all Node|

## ReqNodeList
This message has following fields

`type`:This field has `ReqNodeList`

`id`:This field has Node id who send it.

`status`:This field has Node status who send it. status code is based on this definition TODO:LINK

`address`:This field has Node IP address who send it. if status code is `CONNECTION_NOT_PORTMAPPPED`,this field is not defined.

## ReqNodeAddr
This message has following fields

`type`:This field has `ReqNodeAddr`

`id`:This field has Node id whose send it.

`status`:This field has Node status whose send it. status code is based on this definition TODO:LINK

`address`:This field has Node IP address whose send it. if status code is `CONNECTION_NOT_PORTMAPPPED`,this field is not defined.

`Node_id`:This field has Node id address who send Node designate.

`depth`:This field has the number of how times relay this message. For example, if This field has `0`,the Node who receive this message do not relay message,else,Node who receive this message resend other node with set this field decreased nember.

## ReqRecvBlock
This message has following fields

`type`:This field has `ReqNodeList`

`id`:This field has Node id who send it.

`status`:This field has Node status who send it. status code is based on this definition TODO:LINK

`address`:This field has Node IP address who send it. if status code is `CONNECTION_NOT_PORTMAPPPED`,this field is not defined.

`depth`:This field has the number of how times relay this message. For example, if This field has `0`,the Node who receive this message do not relay message,else,Node who receive this message resend other node with set this field decreased nember.

