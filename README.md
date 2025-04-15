By Jack Rogers, 2210081

All features have been implemented, matching the specification requirements for each action and their effects on expenses, categories, and items. No additional features have been implemented beyond the initial spec, and testing has been done to ensure possible exceptions are caught and dealt with as much as possible with appropriate error messages matching the format of the specification.

I have utilized lists for storing items and categories due to the fast insertion and removal, and as automatic
sorting is needed by the specification and only needs to be done for comparisons

I have commented the code to explain some design choices and give clarity where suited, but I have left todo comments in tact as the specification requests, however was I to work on this project in my own time I would amend these with more specific details on implementation details such as with a Javadoc equivalent.

No external libraries were used and only the standard library and what was provided with the framework.
Any additional methods not specifically asked to be created are at the bottom of each class, these typically serve to minimize code repetition, getters for private variables as is best practice, and to allow features such as .sort() to work correctly