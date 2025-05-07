### Observer Design Pattern 

This document describes a simplified version of a message queue system which implements event driven design through message-based events. My company has an application that has thousands of distributed systems that can message our server at any time, and up to millions of messages a day. These messages all go through a single message broker which maintains a list of dependents and notifies these other components on the server to do certain actions. 

 
This system is a perfect case for the observer pattern: 

The observer pattern is a software design pattern in which an object, named the subject, maintains a list of its dependents, called observers, and notifies them automatically of any state changes, usually by calling one of their methods. It is often used for implementing distributed event-handling systems in event-driven software. 

![image](https://github.com/user-attachments/assets/18b3caa1-0d86-4b7c-9b8a-ac948aebccc9)

The above class diagram depicts the four classes in this system and relates them to the nominal design pattern. The methods in red are the observer typical methods, and the methods in blue are implementation specific to this implementation of a message queue.
