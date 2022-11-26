# WinUSSR Engine
### I think it works now? I don't fucking know anymore
### This is made in 32 bit mode sorry 

This is a library aimed at making payloads for windows targets a bit easier (I guess faster too?)

Note: It is recommended that you use visual studio on windows. If you aren't, you may want to create a makefile for this garbage if you plan on using it.

To get started using this:
- Include in a project add a ref
- do some setup
- create an entry point that at some point has the following lines of code:
-```c++
  auto Sandbox = WinUSSREngine::CreateApp();
  Sandbox->run();
  delete Sandbox;
 -```
Finally: I recommend using the shellcode stuff, the tcp code was used from Lilith Rat and rewritten into my own code style with some added features (I.e. A keylogger lmfao)
         however it's fucking garbage in my opinion. Use at your own risk.
         
# DO NOT USE THIS FOR ANY ILLEGAL ACTIVITIES, THIS WAS MADE FOR EDUCATIONAL PURPOSES ONLY!!! I DO NOT CONDONE USING THIS FOR ILLEGAL ACTIVITIES
### i already got my phd in psychology because of this project, I don't recommend you do the same lmfao
