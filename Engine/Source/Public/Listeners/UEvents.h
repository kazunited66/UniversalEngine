#pragma once
#include "EngineTypes.h"


//System Libs 
#include<functional>

template<typename... Args>
class UEvents {
public: 
	// adding a function into our callbacks array
	UUi8 Bind(const std::function<void(Args...)>& callback) {
		//make a unique structure or UCallback 
		auto newNode = TMakeUnique<UCallbackNode>();
		//move the function into the unique struct 
		newNode->callback = std::move(callback);
		//this will give us the index for the callback as push_back adds the element to the end
		UUi8 id = 0;
		if (m_callbacksNodes.size() > 0) {
			//find an id to assign to the callback node
			//start with 1 ID 
			UUi8 potentialID = 1; 

			//looping through each number to test if an id exists
			while (id == 0) {
				bool foundId = false;
				//if a node has that id set found to false and break out of the for loop 
				for (const auto& node : m_callbacksNodes) {
					if (node->id == potentialID) {
						foundId = true;
						break;
					}
				}
				//if no id matchs the potential id the beak the while loop and set id 
				if (!foundId) {
					id = potentialID;
					break;
				}

				//if an id was found that matched increase to the next potential 
				++potentialID;
			}
		}
		else {
			id = 0;
		}
		
		newNode->id = id;
		
		//add the unique callback into array 
		m_callbacksNodes.push_back(std::move(newNode));

		return id;

	}
	//run all functions bound to this event listener 
	void Run(const Args...args) {
		//loop through all of the stored functions 
		for (const auto& node : m_callbacksNodes) {
			//run each function with the argument 
			node->callback(args...);
		}
	}
	//unbind a function based on the index 
	//get the index from the initial bind 
	void Unbind(const UUi8& index) {
		std::erase_if(m_callbacksNodes.begin(), m_callbacksNodes.end(),
			[index](const UCallbackNode& node) {
				return node->id == index;
			}
		);
	}

private: 

	struct UCallbackNode {
		std::function<void(Args...)> callback;
		UUi8 id;
	};

	//storing function to run when the event runs 
	TArray<TUnique<UCallbackNode>> m_callbacksNodes;

};

typedef UEvents<> UEventsVoid;