# WinUSSR Engine
### I think it works now? I don't fucking know anymore
### This is made in 32 bit mode sorry 

This is a library aimed at making payloads for windows targets a bit easier (I guess faster too?)

Note: It is recommended that you use visual studio on windows. If you aren't, you may want to create a makefile for this garbage if you plan on using it.

To get started using this:
- Include in a project add a ref
- do some setup
- create an entry point that at some point has the following lines of code:



```
    auto Sandbox = WinUSSREngine::CreateApp();
    Sandbox->run();
    delete Sandbox;
```

<bold>Optionally you can just look at how ShellDll is setup in order to get an idea of how to use this garbage fucking library.</bold>

Finally: I recommend using the shellcode stuff, the tcp code was used from Lilith Rat and rewritten into my own code style with some added features (I.e. A keylogger lmfao)
         however it's fucking garbage in my opinion. Use at your own risk.
         
# DO NOT USE THIS FOR ANY ILLEGAL ACTIVITIES, THIS WAS MADE FOR EDUCATIONAL PURPOSES ONLY!!! I DO NOT CONDONE USING THIS FOR ILLEGAL ACTIVITIES
### i already got my phd in psychology because of this project, I don't recommend you do the same lmfao

Thank you to the people who made Lilith RAT for giving me a fun programming exercise. I decided to rewrite your rat into my own style and see if I could add or just overhaul features and see if I can overhaul how it works to begin with.
Lots of this wouldn't have been possible though from the many, many people who gave tips & tricks along the way as well. 
Many thanks to them as well. <3
