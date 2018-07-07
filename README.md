# ITZ-Library

The goal of this project is to study how ARM TrustZone can be used as a secure environment, in order to inspect the memory of a running system. 
The idea is to explore this technology and combine it with the concept of virtual machine introspection.
The normal world of the device is responsible for maintaining the Linux kernel and the applications running. 
We consider this world insecure and easily compromised, and therefore it cannot inspect itself.
Taking this into account, we adapted the idea of VMI, which is a technique used to inspect and analyze the code running on a given virtual machine. 
The way we do the introspection is by using our introspection library running from the secure world.
