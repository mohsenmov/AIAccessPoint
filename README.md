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


## Insights on choices: (documentation)
the dev choices in this project were to be the most time efficient solution to the challange provided by the assingment. it was also limited by my personal's device struggle to run heavy programs such as ollama and UE5 at the same time. hence functional was the choice, instead of fancy.
### AICommunication scripts:
- the SendMEssage function initially was set up to use a json helper function for the content of the request however this seemed to confuse ollama and so I chose to write it manually in one script
- It was much more straight forward to Bind a callback once the request is complete, OnRecieveMEssageFromAIResponse() function, so that the entirety of the http communcation and response access was done in one flow. as a result I set up the ReceiveMessageFromAI function to detect whether a response has been saved to a global variable using the Tick function, and display the results. This might be an alternative task compared to the assignment description, however practically, when used in a blueprint, the node will end up behaving the similarly by returning the response string.
[the UIPanel widget blueprint](https://github.com/user-attachments/assets/6a311c6f-a676-4a6f-866a-3ad7658be81e)
- the POST api/generate access point in ollama was used instead of the POST api/chat. this is mainy due to the fact the consequent requests in a chat api require stitiching the history of the meesages. while this is possible, it was deemed to time inefficient and not neccesarily the purpose of the project.
- 
