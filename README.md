# OLlama AI Access Point
## Overview
This project is a simple UE5 UI for sending in prompt to the local Ollama server using HTTP REST Api provided by Ollama.
## how to use
Run Unreal engine.
navigate to the blankLevel in the project.
Play the unreal project and simply put your prompt into the prompt box. the press submit. if the response is successful the response box will update with the response recevied from the server.
## Set up
### set Ollama up
Install ollama, download llama3.2 model.
Start an ollama server at http://localhost:8000 using the command line. 
command = ( OLLAMA_HOST=localhost:8000 ollama serve)
This instantiates a server at this address and is ready to receive requests.

## Notes:
- This project uses the Generate Completion API with stream set to off.
- If device has outdated GPU or is over worked, Ollama will sometimes respond with empty results or errors, or results unrelated.
- (to be tested on a better device)
- this project uses the native http and json utility already on UE5 engine. To be improved with external packages
- make sure you read the logs for further info on errors coming back
