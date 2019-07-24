# 2 Messages, Classes and Servers

  前一章使用标准消息类型和publisher-subscriber机制实现异步通讯.本章介绍点到点通讯:client-Services机制.client-Services交互会address concerns of publish and subscribe in terms of knowledge of the source of communications and guaranteed receipt of messages.client-Services机制的限制在于传输是阻滞的,client节点会被挂起,直到service节点响应.还有第三种通讯机制:action clients 和 action servers.

## 2.1 定制消息
