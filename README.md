Software Sharing Over Private Cloud

  This Project is an attempt to share resources that in this case are Software on a cloud.
An adhoc overlay network is created when a node-server is started. Every node carries a list nearest nodes. Nearest nodes are identified on the basis of cost associated to them.

Cost-   These cost are determined by sending an icmp packet, and calculating the round trip time.
These associated cost  of neighbor nodes are stored on each node along with their address. They are sorted according to the cost. Software request is send to each node, according to the sorted sequence.


Sender

	When a particular Software is not available at the client machine, request can be sent to  the cloud. The request is routed within  the cloud. If the required software is found available on some server in the cloud, the given input is processed an d the output is generated. The generated output is 
send back to the requesting client. 
	Sender node carry a list of neighbor or least cost nodes in the cloud. Each node is picked from the sorted list. Request is sen to each node one by one. If the software is found available on any of the server, process exits, No further request is sen to any node.
	Every time a s/w is found unavailable on a server, a nack is send back to client. Further, the same request is forwarded to its neighboring node. This way the request get routed within the whole cloud.
	When a positive response for the routed request is received from some server within the cloud,
the neighbor nodes list is updated.

Reciever 

	Any Node-server within the cloud acts as a Reciever. It is always in the waiting state, waiting for a request. When a request is received, a new process is forked to process that request.
Request message is read and parsed and stored in a data structure. Attributes of request is determined.

Primary Request-  
	The primary request is one that is received directly from the sender, ie the server must be listed within the least-cost-node list of sender. If the request is primary, then the software is checked for availability. If found available an acknowledgement is send back . The s/w is executed on the server side, and the output is generated. This output is written back to the requesting client. On the sender , the output is presented to the end user by formatting it .

Secondary Request-  
	The Routed request within the cloud that is received by some node-server is the secondary request. The  source address (ie, the address of originally requesting client), is read and a connection is established with the source sender. Sender thus obtain the the address of an available node within the cloud. It updates its least-cost-node list. Software is executed on the node-server. The output is send back , which is presented to the end user.



Presentation

	The Software request when executed on a node-server generates some output. This output need to be communicated to the sender node. Further the output also need to be presented to the end user.
	At the node-server(available reciever) the output generated is formatted into a valid format according to the type of software. Since different softwares executes differently and generates output in different format. Thus for every type of Software there is implemented a unique presentation layer. This
formatted output file is written on the requesting sender.
	On the sender side, the output is formatted into some presentable format. In this case, it is converted into a HTML format. Output is presented to the end user, in HTML document through a web browser. 


Data Structure-

1.	A Bucket data structure is used to store least-cost-node list (neighbor node) list at each node. An array is maintained with its index value as the cost of nodes. At each index a linked list of nodes is built ,with corresponding cost value.
2.	A linked list is maintained to store the list of all available software at each node.


Shared Software-

1.	Simple applications like- addition, subtraction, multiplication,division,
2.	gcc compiler and java compiler. 

