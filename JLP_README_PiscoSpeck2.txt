AndorSpeck2 main process : asp2_main.cpp
ID_PTHREAD_START1 : StartProcessCube1Event
ID_PTHREAD_START2 : StartProcessCube2Event

JLP_DecodeThread: asp2_threads.cpp 
ID_PTHREAD_DONE : CubeIsProcessedEvent

JLP_AndorThread: asp2_threads.cpp 
ID_ATHREAD_DONE : CubeIsLoadedEvent

************** ANDOR/PROCESS threads *************************************

send load cube 0 
Cube_is_loaded[1] = false;
Cube_is_loaded[2] = false;
Cube_is_processed = true;

when received loaded cube
 1. get cube 1 or 2 if Cube_is_loaded[1] = false of Cube_is_loaded[2] = false 
copy to Cube1 and then Cube_is_loaded[1] = true 
or  
copy to Cube2 and then Cube_is_loaded[2] = true;
 2. if(Cube_is_processed == true) send this cube to process load cube1 
    and set Cube_is_processed = false
 3. if(Cube_is_loaded[1] = false or Cube_is_loaded[2] = false) send load cube

when received processed cube
Cube_is_processed = true;
 1. if(Cube_is_loaded[1] or [2] = true) send process load cube1 or 2 
 2. send load cube 1 or 2

