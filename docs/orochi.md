# What's is Orochi

Orochi is p2p network system for multi purpose.based on IPFS but some features are diffrent.

## Whats's diffrent form IPFS or other P2P

Orochi is based on IPFS. However I feel disability of that. Concreatly

- IPFS is not real time content. (such as video chat)
- IPFS is not supported some enviroment. (IPFS is alomostly wrtten by Golang)
- IPFS don't have traceability.

Therefore,Orochi is aimed to overcome these weakpoints about IPFS

# Orochi's workflow
orochi nodes are working following workflow.

```mermaid
flowchart TB
Initialize --> Request
````

## Initializing 
At first orochi's nodes are initialzing.this process can be divided 4 term.

First nodes initialize node's table. node's table recodes other node's address and id.if this table is empty,nodes get it from discovery server

Sencond nodes broadcast there address which set with there id.
