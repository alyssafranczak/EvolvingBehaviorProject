
#pragma once

#include "Modules/ModuleManager.h"

/*! \mainpage EvolvingBehavior Documentation
 *
 * An AI-assisted tool to experimentally refine behavior trees through interactive evolution.
 * 
 * Authors: Nathan Partlan (NPCDev), Jim Howe, Luis Soto, Sarthak Shrivastava, Alex Grundwerg, Isha Srivastava
 * 
 * Additional collaborators: Magy Seif El-Nasr, Erica Kleinman, Sabbir Ahmad, Muhammad Ali, Zheng Fang, Stacy Marsella
 * 
 * Special Thanks: Tom Looman
 * 
 * The Evolving Behavior plugin uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc. All rights reserved.
 * 
 * \section intro Introduction
 * 
 * This is a plugin and tool for use with Unreal Engine 4. It works on top of the existing Unreal Engine 4 behavior tree technology to evolve new behavior.
 * 
 * You will need to provide a basic behavior tree, and specify various additional nodes and properties that could be generated and added to the tree. Then, you will need to specify how your behavior trees should be tested: requesting trees to be generated for agents, how their fitness should be calculated, and how long each experiment should last.
 *
 * By running the experiment and interacting with the generated agents to test their fitness, you will then help to guide the evolutionary process towards behavior that fits your goals. 
 * 
 * \section installing Installing
 * 
 * Copy the EvolvingBehavior plugin folder to the "Plugins" directory in your Unreal Engine 4 project folder.
 * 
 * Alternatively, to install as an Engine plugin for all projects, copy to the "Plugins" directory inside your Unreal Engine 4 engine folder.
 * 
 * You may need to recompile your project and/or restart Unreal Engine 4.
 * 
 * For more information, see: https://docs.unrealengine.com/en-US/Programming/Plugins/index.html
 * 
 * \section setup Setting Up an Experiment
 * 
 * Next: \ref page_setup "Setting up an experiment to evolve a behavior tree..."
 */

/*! \page page_setup Setting Up an Experiment
 * 
 * \section lifecycle The LifeCycleActor: Managing the Experiment's Life Cycle
 * 
 * The \ref ALifeCycleActor "LifeCycleActor" controls how the evolution progresses: its timing, fitness measurements, and so forth. To start using EvolvingBehavior, you should add exactly one LifeCycleActor to your scene. You will then want to set several properties on it, to determine how fast and how long your experiment should run, among other details.
 *
 * \subsection lifecycleproperties Life Cycle Properties
 * 
 * ![An example of setting properties for the Life Cycle component.](LifeCycleProperties.png)
 *  
 * You can set various properties to control the evolutionary life cycle.
 * 
 * \li __Trial Length__ (in in-game seconds): This determines the time limit for each generation of the evolution. When this time expires, the scene will be restarted with a new generation of behavior trees. Note that this is in-game time, not real-time, so even if you change the Game Speed Modifier, the trial will still simulate the same period of gameplay.
 *
 * \li __Total Trials__: The number of generations over which to run the evolution. After this many generations, the experiment will end.
 * 
 * \li __Trial Size__: The number of behavior trees to generate in each generation. This should ideally be the same as the number of AI agents who will be spawned in the level to test each individual, generated behavior tree.
 * 
 * \li __Game Speed Modifier__: This multiplies the game speed. You can set this higher to accelerate the experiment, but note that setting it too high may cause strange behavior in the physics system.
 *
 * \li __Experiment Ending Map__: The map to load when the experiment is completely finished.
 * 
 * The LifeCycleActor also controls \ref page_results "Saving and Loading," which will be described later along with its associated settings.
 *
 * \section evolutioncontrolactor The EvolutionControlActor: Specifying Settings for Each AI Population
 * 
 * The \ref AEvolutionControlActor "EvolutionControlActor" is the invisible actor that will control how EvolvingBehavior works for a specific population (type) of AI agent in your experiment/game. To begin using the plugin to evolve a type of AI, first add an EvolvingControlActor to your scene. If you have multiple types of AI to evolve, you can add one EvolutionControlActor per AI type, and set their settings individually.
 * 
 * To associate an EvolutionControlActor with a particular type of AI, the easiest way is to set a Tag on the EvolutionControlActor with the name of that population/type. Separately, in code or blueprints when spawning actors for that specific type of AI, you will then find the specific EvolutionControlActor by tag, and ask it to provide behavior trees for that specific AI type (or, instead of using tags, you could also search by name, or set a direct refrence to the EvolutionControlActor). We will show example code for this later, when talking about \ref spawningnpcs "Spawning NPCs."
 * 
 * The EvolutionControlActor contains the \ref evolutionmanager "Evolution Manager Component," which provides the settings for the population.
 * 
 * If you need access to parts of these components, such as the Population Manager and the Fitness Tracker, you can retrieve the EvolutionControlActor from the scene and access its components. We provide a helper method that can be called from C++ or Blueprints (defined in EvolutionControlActor.h), called GetEvolutionControlActors, to easily retrieve the evolution control actor from the scene.
 * 
 * \section evolutionmanager The Evolution Manager Component
 * 
 * The next step is to set up the BTEvolution Manager component. It will also be automatically created on the EvolutionControlActor.
 * 
 * The BTEvolutionManager component handles the evolution process of the "population" of Behavior Trees evolved from your initial, hand-designed tree.
 *
 * You will need to specify the following sets of properties:
 * 
 * \li The \ref fitness "Fitness Calculator" for measuring the performance of each agent and determining their fitness for serving as parents to the next generation.
 * 
 * \li The \ref templatecollection "Template Collection" containing the starting behavior tree and extra nodes.
 * 
 * \li The \ref generatedtemplatelibrary "Generated Template Library" containing the information to generate completely new nodes of various kinds.
 * 
 * \li The \ref reproducer "Reproducer" for creating new behavior trees out of old ones.
 * 
 * \li The \ref parentselector "Parent Selector" for choosing which behavior trees to use as parents, based on their fitness, for creating the next generation.
 * 
 * \li The \ref initialpopulation "Initial Population" settings for randomly mutating your hand-designed behavior tree, if desired, to create more variety in the starting population.
 *
 * \section fitness Determining Fitness
 * 
 * Fitness calculation is important for guiding the evolution of your behavior tree. You will want to try to define fitness functions that determine the degree to which each behavior tree's actions meet your goals. It can be difficult to capture all of your goals in numeric calculations, however, so you may need to iterate on your fitness functions and try various approaches. Inspecting and interacting with the resulting agents may also help you determine how well the resulting agents meet your needs, even if the fitness functions are not perfect.
 *
 * ![An example of a Linear Weight Fitness Calculator with several different keys of various weights.](ExampleFitnessCalculator.png)
 * 
 * You will need to specify a Fitness Calculator in the Evolution Manager Component. The Fitness Calculator controls how all the different fitness calculations are combined to determine the final fitness for the agent. Usually, you can use the built-in \ref linearweight "Linear Weight Fitness Calculator", which adds together all fitness calculations. If that does not meet your needs - for instance, if you want differently-curved fitness calculations - you could write a new Fitness Calculator.
 * 
 * To set up a new Fitness calculation, you need two things:
 *
 * \li A property entry in the FitnessCalculator on the EvolutionManager component, set up with details for each fitness key. The key name will be used by the code to update the fitness value for that agent. See \ref linearweight for more details on setting up the standard calculator.
 * 
 * \li C++ or Blueprint code that calls the "BroadcastFitnessUpdated" event on a FitnessUpdater. (To see more detailed versions of the C++ and Blueprint example code below, you can download, run, and examine the EvolvingBehavior Example project.)
 *
 * In Blueprint, you can register a fitness updater as follows:
 *
 * ![An example of registering a fitness updater in Blueprint.](BlueprintRegisterFitnessUpdater.png)
 *
 * In the shown Blueprint code above, we get or add a Single ID Fitness Updater component, and then call "Register Fitness Updater" on the EvolutionControlActor for this agent's population.
 *
 * And here is an example of Blueprint code to send a fitness update:
 *
 * ![An example of sending a fitness update in Blueprint. This function can then be called from other blueprints, sending specific fitness keys and values.](BlueprintSendFitnessUpdate.png)
 *
 * In the shown Blueprint code above, we get our Single ID Fitness Updater component, then call its "Broadcast Fitness Update" function, passing a specific Fitness Key (a String naming the aspect of fitness we want to update), a floating-point new value, and a bool that is true if the new value is a "delta," meaning that it should be added to the previous value, or not (meaning it should directly replace the previous value). In this way, we can either incrementally modify fitness over time, or track it ourself and set the entire value at once.
 *
 * In C++, this code should find the FitnessTracker for the population, register the FitnessUpdater (which must implement IFitnessUpdater), then call BroadcastFitnessUpdated:
 * \code{.cpp}
 * 
 * UFitnessTracker* fitnessTracker = populationManager->GetTracker();
 * fitnessTracker->RegisterFitnessUpdater( this );
 * 
 * // Adds 1 to the current fitness value for the "Alive" fitness key.
 * // You will need the ID of your NPC's behavior tree, which you will get and save when you Register the use of that behavior tree from the population (see below section on retrieving behavior trees).
 * BroadcastFitnessUpdated( behaviorTreeID, FString( "Alive" ), 1, false );
 * 
 * \endcode
 * 
 * \subsection linearweight Linear Weight Fitness Calculator
 * 
 * The Linear Weight Fitness Calculator is a built-in, simple fitness calculator. It multiplies each fitness key's value by a corresponding "weight," then adds the result together.
 * 
 * For example, if you had two keys and weight values, "MovementDistance: 2.0" and "PointsScored: 3.0", and the result for the agent was 10 movement distance and 4 points scored, the final fitness for that agent would be: 2 * 10 + 3 * 4 = 32.
 * 
 * You should set up the keys and their corresponding weights in the "Id to Weight Map" property field.
 * 
 * \subsection templatecollection Template Collection
 *
 * ![An example of setting properties for the Template Collection.](TemplateCollectionProperties.png)
 * 
 * The Template Collection contains the manually-designed behavior tree(s) and the extra library(s) of additional behavior tree nodes you would like the evolutionary process to work with.
 * 
 * You can specify one or more starting behavior trees in the "Initial Population Templates" list. These will be the behavior trees used for the first generation of agents, or, if you specify a \ref initialpopulation "Initial Population" mutation/generation process, they will be used to start the random mutations that generate the starting population.
 *
 * The "Extra Templates" are additional behavior trees, but the structure of these trees will be ignored - they will be used as sources of additional pre-set nodes that can be swapped into the behavior trees during certain types of mutation. In this way, these "extra templates" act as "libraries" of nodes to try to place in the real behavior trees.
 *
 * \subsection generatedtemplatelibrary Generated Template Library
 * 
 * The Generated Template Library contains the information needed to randomly create completely new behavior tree nodes with various properties.
 *
 * ![An example of a specification for a Generated Task Node, showing a float property and a blackboard key property.](GeneratedBehaviorTreeNode.png)
 * 
 * You could use this to allow the evolution to add, for instance, a Wait node that has a random time between 0.5 and 3 seconds, and which may mutate over time to have various values in that range, in later generations.
 *
 * The nodes are separated by type (Task, Decorator, or Service - currently, generated Composite nodes are not supported). In each list, you can add multiple nodes of the appropriate Generated node type, with the following properties:
 * 
 * \li __Name__: The node's name, used to show the node's information and help you recognize it.
 * 
 * \li __Class Type__: The actual Behavior Tree node class that should be generated.
 * 
 * \li __Property Dict__: The properties that should be generated inside the node.
 * 
 * The Property Dict has a set of Generated Properties of various types (float, int, bool, etc.)
 * The "name" of each property must match the name of a real property on the Behavior Tree node type that is being generated (no spaces).
 * The properties, depending on type, may also have a "value" and/or "lower bound," "upper bound," or set of possible values to fill in, to specify what reasonable values may be when randomly mutating them.
 *
 * For Blackboard properties, you must additionally specify which Blackboard they should use to determine possible values, and which type of Blackboard variables they should allow.
 *
 * 
 * \subsection reproducer Reproducer
 *
 * ![An example of setting properties for a Mutating Reproducer.](MutatingReproducerProperties.png)
 * 
 * The Reproducer controls how specific Behavior Trees (usually a pair, selected by the \ref parentselector "Parent Selector") are reproduced and mutated to create a new Behavior Tree for the next generation. Usually, a "Mutating Reproducer" will be flexible enough for most purposes. However, it is also possible to write new types of Reproducer in C++.
 * 
 * In addition to a reproducer, you may also specify a value for "Elitism," which is the process of directly copying a percentage of the most fit individuals from one generation to the next, unchanged (without any mutation). This helps to avoid loss of successful behavior due to random mutation, by ensuring that the most effective individuals from the previous generation continue to have a chance to reproduce, even if none of their offspring were successful. We recommend an "Elitism" of between about 5-20%, depending on the size of the population and volatility of mutation.
 * 
 * \subsubsection mutatingreproducer Mutating Reproducer
 * 
 * In a Mutating Reproducer, you can add any number of Mutators and give them specific probabilities of acting to change the child that is created.
 * 
 * The __Crossover__ mutator is a common choice. It picks a random point in each of the two parent Behavior Trees, and replaces the primary parent's sub-tree with the branch selected from the secondary parent, essentially grafting the branch to create a child that is a hybrid of the two parents. The __Depth Biased Crossover__ mutator works similarly, but tends to select nodes closer to the leaves of the tree with a particular probability bias you can set using a property. This could be useful if the standard Crossover mutator is making too many large changes to the tree by selecting central trunk nodes.
 * 
 * Additionally, there are many mutators available that Add, Remove, or Replace random nodes (of particular types) in the child. These are called "point" mutations, because they act on a single node instead of an entire branch.
 * 
 * Some mutators always use nodes from the "Extra Templates," the hand-created nodes in the \ref templatecollection "Template Collection" described above. These have "Mapped" in their name. Others always use nodes from the \ref generatedtemplatelibrary "Generated Template Library," generating properties based on the rules there. These have "Generated" in their name. Finally, some mutators will randomly select between Mapped or Generated, with a probability you can set.
 * 
 * There will also be mutators for modifying properties of existing nodes, rather than replacing the nodes entirely. These are a planned feature to be released in a future version of the tool, but they are not yet available at the time of writing.
 * 
 * \subsection parentselector Parent Selector
 *
 * ![An example of setting properties for the Tournament Selector parent selection algorithm.](ParentSelectorProperties.png)
 * 
 * The Parent Selector chooses the specific chromosomes to be used as parents for the next generation, and these parents are sent in batches (usually pairs) to the Reproducer.
 * 
 * \subsubsection Tournament Selector
 * 
 * The most common Parent Selector is the __Tournament Selector__. This parent selector randomly selects some number (we'll call it K) of possible nodes from the population. Then, it takes only the highest-fitness node of that small subset. This gives almost every member of the population a chance to be selected, but over time, the more fit parents are more likely to win each small tournament. The larger the size of the tournament (K), the more likely that a high-fitness chromosome will be in the list. Therefore, high values of K will tend towards always selecting high-fitness parents, and lower values will allow more variety and mix in a larger number of less-fit parents. Often, 4 is suggested as a good tournament size (K) to try first.
 * 
 * \subsection initialpopulation Initial Population
 *
 * ![An example of setting properties for the Initial Population.](InitialPopulationProperties.png)
 * 
 * The Initial Population properties provide a way to randomly mutate the input Behavior Trees before starting the experiment, creating more random variety in the initial chromosomes used for the experiment (making variations of the original behavior tree randomly, without regard for fitness).
 * 
 * \li __Num Initial Population Generations__: determines how many generations of random children to create before starting to test fitness.
 * 
 * \li __Initial Population Reproducer__: allows you to configure a separate set of mutators with different probabilities, so you can add more random mutation, or put more limits on the changes, relative to the rest of the experiment generations.
 * 
 * If you set the number of initial population generations very high, the behavior trees may be very different from your original behavior tree. This might be good for initial prototyping and experimenting, but less helpful if you are doing minor tuning on a behavior tree. If you find that all your behavior trees are too similar at the start of the experiment and you end up with the same results all the time, try adding more probability to the mutators (or more mutators), and/or increase the number of initial generations.
 *
 * \section spawningnpcs Making NPCs Use Behavior Trees From The Population
 * 
 * You will need to test each Behavior Tree generated as part of the Population, in order to find out how fit each evolved Behavior Tree is.
 * 
 * To do this, when you spawn an NPC (or you could switch the behavior tree running on an existing NPC), you can retrieve the next available Behavior Tree from the current Population generation, and set that Behavior Tree to run on the NPC. (To see more detailed versions of the C++ and Blueprint example code below, you can download, run, and examine the EvolvingBehavior Example project.)
 *
 * ![Example Blueprint code to register a newly spawned bot and retrieve a behavior tree - see next screenshot for details of the custom function "Run Tree and Register Fitness" defined in Blueprint.](BlueprintRegisterBehaviorTree.png)
 *
 * In the Blueprint code above, we first find the EvolutionControlActor with the tag for this pawn's Population, then call "Try Retrieve Next Pop Member" on that EvolutionControlActor. That function returns a "PopulationReg" object with this pawn's new ID (for tracking fitness), and associated Behavior Tree from the population. If we can't get a registration immediately (if the Population is not yet initialized/generated), we can register for an event that will tell us when the Population is ready for us to try again.
 *
 * In Blueprint, you can run the behavior tree as follows:
 *
 * ![The details of the "register behavior tree" function in Blueprint.](BlueprintRunBehaviorTree.png)
 *
 * In the Blueprint code above, we call the "Run Behavior Tree" function on our pawn's associated AIController, passing the Behavior Tree we received from registering with the Population. Then, we make and register a Fitness Updater (that Blueprint code is shown above in the \ref fitness "Fitness" section).
 * 
 * You can also do this in C++, rather than Blueprint. Here is example C++ code to retrieve the behavior tree and set it on the AI Controller for the NPC:
 * 
 * \code{.cpp}
 * 
 * // Find the EvolutionControlActor and get the Population data.
 * // If there's more than one with this tag, just use the first.
 * TArray<AEvolutionControlActor*> evolutionControlActors; 
 * AEvolutionControlActor::GetEvolutionControlActorsByTag(GetWorld(), "DesiredPopulationTag", evolutionControlActors);
 * AEvolutionControlActor* evolutionControlActor = evolutionControlActors[0];
 * UPopulationManager* populationManager = evolutionControlActor->GetPopulationManager();
 * 
 * // Check that there is a behavior tree available, and register that you are now using the next available one.
 * FPopulationReg btRegistration;
 * if( nullptr == populationManager || !populationManager->IsTrialReady() || !population->Register(reg))
 * {	
 *  // No behavior tree is available - you could run fallback code to insert a standard behavior tree, or register for the event populationManager->OnTrialPopulationReady() and wait for the callback.
 *  return;
 * }
 * 
 * // You will need to write code to keep track of which NPC is registered to this behavior tree, so you can send fitness updates with the correct ID.
 * // For instance, you could have a Pawn subclass that keeps a variable to store this ID.
 * StoreNPCBehaviorTreeID(this, btRegistration.id);
 * 
 * UBehaviorTree* newTree = btRegistration.bt;
 * 
 * // This is only necessary if you need to stop an existing tree that is running on your NPC.
 * if( BehaviorComp->TreeHasBeenStarted() )
 * {
 * 	BehaviorComp->StopTree( EBTStopMode::Safe );
 * }
 * 
 * // Set up the new tree's Blackboard.
 * if( newTree->BlackboardAsset )
 * {
 * 	BlackboardComp->InitializeBlackboard( *newTree->BlackboardAsset );
 * }
 * 
 * // Run the new tree on your NPC!
 * BehaviorComp->StartTree( *newTree );
 *
 * \endcode
 *
 * \section unrealsettings Unreal Engine and OS Settings for Running Experiments
 *
 * You may want to change some Unreal Engine and computer settings in order to run EvolvingBehavior experiments in the background. By default, both Unreal and Windows will de-prioritize the engine while it's not the main focused window, causing framerates to slow and experiments to run improperly.
 *
 * The Unreal Engine setting you will want to change is:
 * 
 * \li Editor Preferences -> General - Performance -> Editor Performance -> Use Less CPU when in Background: UNCHECK
 * 
 * If on Windows, the Windows 10 setting you may want to change is:
 *
 * \li Control Panel -> System -> Advanced system settings -> Advanced -> Performance -> Settings... -> Advanced, in the options for "Adjust for best performance of:" select the radial button for "Background services."
 * 
 * Additionally, to run physics-based simulation at high speed, you may want to increase the maximum cap on movement simulations per tick. You will need to do this separately for each type of actor you are testing. You can change these settings in the individual Pawn blueprint class, under "Character Movement (General Settings)":
 *
 * \li Max Simulation Iterations - increase up to 16
 * \li Max Simulation Time Step - if increasing iterations is not sufficient, you may also want to change this value
 * 
 * \section nextsteps Next Steps
 * 
 * Now that you have started an experiment, you can play against the evolving NPCs (or create opponent AIs to play against them), and then save the results of the experiment.
 *
 * To learn how to save and look at the results of your experiment, continue reading:
 * 
 * Next Page: \ref page_results "Experiment Results: Saving, Loading, Evaluating".
 * 
 */

/*! \page page_results Experiment Results: Saving, Loading, Evaluating
 *
 * Once you have \ref page_setup "set up" your experiment, you may want to save the results, load them and resume the experiment, or evaluate and use the behavior trees.
 * 
 * \section ending Ending the Experiment
 * 
 * The experiment will end after the specified number of trials (generations) from the \ref lifecycle "LifeCycle" Actor. You can also interrupt the experiment early by stopping the game, and you can then \ref savingandloading "load" and resume it later if you wish.
 * 
 * You can set a specific Map to load when the experiment ends, as well as the map to load to restart the level when each individual Trial ends, in the \ref lifecycle "LifeCycle" component.
 * 
 * Note that, if you stop an experiment in the middle of a Trial, that partly-completed Trial will *not* be saved.
 * 
 * \section savingandloading Saving and Loading the Experiment In Progress
 * 
 * After testing every generation (a single trial), the experiment will automatically save its state. If you stop running the experiment and then start it again, it will automatically load and pick up at the beginning of the next generation/trial.
 * 
 * ![The settings for saving and loading an experiment in LifeCycle.](SaveAndLoadSettings.png)
 *
 * In the \ref lifecycle "LifeCycle" Actor, you can change properties to affect the saving and loading:
 *
 * __Slot Name__: Changes the name of the save file to use for saving and loading.
 * 
 * __User Index__: If you want to save multiple experiments into the same file, you can change this number to switch between them.
 * 
 * By changing these to new values, you can switch which experiment you are loading and saving to, or start a totally new experiment while keeping your old ones available.
 * 
 * You can also delete the currently-specified save file by clicking "Delete Current Save." (The save files are placed in the "Saved/SaveGames" folder if you want to manually delete them, copy them, etc.)
 * 
 * \section savingbehaviortrees Saving and Examining Behavior Trees
 * 
 * Each behavior tree evolved to populate each trial will be saved in a folder corresponding to the Trial Number (the number of generations that have been generated so far).
 * 
 * You can find all of these saved trees listed by ID in the /Content/EvolvingBehavior/Trial<num>/ folder in the Asset Browser or on disk.
 * 
 * The filename also indicates the \ref fitness "Fitness" of the tree, to help you sort through them.
 * 
 * Thus, the full filename for a specific behavior tree is: <fitness>-Tree-<id>.asset
 * 
 * You can load these behavior trees in the Unreal Editor as you would any other Behavior Tree asset, though they will be shown with an automatically-generated layout that may not exactly match your preferred placement of the nodes. You may need to manually drag nodes around to look at the tree with your preferred layout.
 * 
 * You can also manually edit these behavior trees, and you can use these behavior trees as static assets on any NPC, by dragging them into the Behavior Tree property for the AI Controller. Essentially, they work just like hand-crafted behavior trees.
 * 
 * You may note that the first time you open the Behavior Tree, it shows as having unsaved changes. This is due to the Editor needing to generate the initial, automatic layout of the nodes. You can safely save the layout without affecting the basic workings of the tree.
 * 
 * If you open a behavior tree immediately after it is generated, you may find that you get an error saying that the tree cannot be fully saved. This is because the tree has not yet been fully loaded in the editor. You can either restart the editor or manually tell the asset to fully load, after which you should be able to save it normally.
 *
 * \section fitnesslogs Fitness Logs
 *
 * You can find the logs of fitness for every generated individual in the trial in the "EvolvingBehavior-FitnessStats" folder inside the "Saved" directory.
 *
 * These logs are stored as .csv files, with the rows corresponding to each generation of a single population. The .csv is named after the population name from the EvolutionControlActor.
 *
 * Note that, if you run the experiment with the same experiment ID again, it will continue appending to the .csv. So remember to change your experiment ID or delete/move the FitnessStats files to avoid mixing results from several runs. (The \ref page_gridsearch "Grid Search" actor separates its experiment IDs automatically, if you want to automate this.)
 *
 * To, for example, create a graph of average fitness over time, you could open the .csv in a spreadsheet editor (such as LibreOffice Calc or Excel), and use =AVERAGE() to average each row into a new column. Then, graph the new average column's values. If the experiment is working well, generally, average fitness will increase in later generations (though it may jitter, stall, or drop temporarily, or eventually plateau and even decline slightly when it runs out of new improvements to find).
 *
 * If you are not seeing improvement over time, you may want to restart the experiment multiple times with different random initial populations, tweak your fitness functions, change which behavior tree nodes are available or generatable, and/or tune various \ref page_gridsearch "parameters" for mutation rates, elitism, etc.
 * 
 * \section additionallogs Additional Logs and Information
 * 
 * You can also find additional information (such as errors and warnings) about the experiment as it is in progress or after it has completed by examining the log files.
 * 
 * The "Output Log" (in Window->Developer->Output Log) contains the relevant log messages. You can also find this log as a text file on disk under your project's directory at "Saved/Logs/". Look for messages with the LogEvolvingBehavior and LogEvolvingBehaviorError categories to find information about the agents and fitnesses, and about errors or problems with the settings, respectively. If you find that agents are not evolving, or the experiment does not seem to be working, these logs are the first place to check for problems. They are also another way to see representations of the behavior trees and their fitnesses without opening each file.
 *
 *
 * \section nextsteps_aftersaving Next Steps
 *
 * Next Page: \ref page_gridsearch "Tuning Parameters".
 * 
 */

/*! \page page_gridsearch Tuning Parameters
 *
 * After you have set up and run an \ref page_setup "experiment," you may find that your experiment didn't quite get the results you expected, or that you want to re-run with several random initial conditions, or otherwise automate the process of running multiple trials. In all of these cases, there may be various parameters you want to change (or you may just want to restart several times and save the results separately). For this purpose, we provide a "Grid Search" actor, to help you automate the process.
 *
 * \section gridsearch Grid Search Experiment Management Actor
 *
 * The "Grid Search Experiment Management Actor" is an optional extra actor you can place in your scene. If you do, you can set it to restart the experiment automatically several times, possibly with various different parameter settings, and save the results in separate folders.
 *
 * ![An example of a grid search for tuning mutation rates.](GridSearchExample.png)
 *
 * You can add various types of "Modifiers" to the GridSearch actor, each of which can change specific parameters, usually through a specified set of values. There is also a "MultiRunNoOp" modifier, which simply restarts the experiment a specific number of times without any changes.
 *
 * The GridSearch actor will run through all the various options set in its modifiers, until every combination of the options has been tested. It will save all the results in folders with labels corresponding to the specific combination of options.
 *
 * Note that, because the search will run through all possible combinations, it will take a long time to run if you use many operators. This means that you will probably want to set the LifeCycle component to a very fast speed, minimize the complexity and length of the trials and the number of generations per experiment, and minimize the number of different types of parameters (and individual parameter settings) you are testing.
 *
 * Currently, we provide the following types of operator (though additional code can subclass and add more types):
 *
 * \li __CrossoverProbabilityParamMod__ changes the probability of all crossover mutators.
 * 
 * \li __PointProbabilityParamMod__ changes the probability of all non-crossover (point) mutators.
 * 
 * \li __MultiRunNoOpModifier__ does nothing but restart the scenario (providing the opportunity to re-run the same experiment with a new random starting population).
 *
 * \section nextsteps_aftertuning Next Steps
 *
 * Next Page: \ref page_troubleshooting "Troubleshooting."
 */

/*! \page page_troubleshooting Troubleshooting
 *
 * On this page, we try to provide some steps to troubleshoot common problems.
 *
 * \section crashes Crashes
 *
 * If you are experiencing crashing, here are some possible troubleshooting steps and common causes:
 *
 * The most common two causes of crashes directly related to EvolvingBehavior are: old/invalid save files, and incomplete or mis-configured Evolution settings.
 *
 * \subsection misconfiguration Misconfigured Evolution Settings
 *
 * \li Check the logs for warnings or errors:
 * \li If you are able to run and pause the experiment at the start before the crash, open the Output Log (Window -> Developer Tools -> Output Log). Search for "EvolvingBehavior."
 * \li After a crash, you can also find these logs in the folder "Saved -> Logs," which will allow you to check if there were warnings or errors before the crash.
 * \li If you see Warnings or Errors, look at what they say. Often they will tell you about some part of the program that is missing data.
 *
 * If you don't see errors in the logs, you can still look through your EvolutionControl actors and LifeCycle actors for settings that are missing (objects set to "None," or missing internal information. Often, adding these missing pieces will resolve crashes.
 *
 * \subsection invalid_save Old or Invalid Save Files
 * If you know that your settings are correct and you were able to run successfully before, but are now crashing, it may be that your experiment's save file is out of date or corrupted. Resuming from a previous saved experiment requires that the settings, behavior trees, and actors be similar enough to copy data from the save file. So if you have upgraded your project or plugin version, or significantly changed your actors or settings, you may need to delete (or rename) the old save (or change your LifeCycle component to a different save name), and start a new experiment.
 *
 * If neither of these resolves your crashes, you may want to try a simpler test case or map, or deleting and re-creating EvolutionControl actors. Your crashes could also be related to your project's code - check that your project runs correctly without EvolvingBehavior being used. Finally, if you are still experiencing crashes, you can submit a bug report to the EvolvingBehavior GitLab repository (https://gitlab.com/NPC-Dev/evolvingbehavior).
 *
 * \section infiniteloops Hangs and Infinite Loops
 *
 * If Unreal Engine is stuck, hanging, or in an infinite loop, a likely cause might be a Behavior Tree node that loops infinitely (or for a long time). The "Loop" node type is a common culprit - make sure you don't have Loop nodes that will run for a long time, or that could generate infinite or long loops, especially if they could nest within each other. If you have created other node types that may loop or take a long time to execute, try removing them as well, and see if the problem stops happening. Then, try to re-write them to prevent infinite or very long execution.
 *
 * \section nothinghapppening Nothing is Changing
 *
 * If you don't see any changes or movement in your characters when running EvolvingBehavior, there are several possible causes. Here are some things to double-check:
 *
 * \li Check that you have built a valid navigation mesh for your level, and that your AI character is set up to move properly on it (perhaps test with a simple, static behavior tree that includes only a "move to.")
 * \li Use the Unreal AI debugger to see that the behavior trees are actually running on your agent, and which nodes are running at any given time. If no tree is running, you might not be retrieving the trees properly when spawning your agents.
 * \li Check for misconfigurations or incomplete settings in the EvolutionControl Actor(s) or LifeCycle Actor(s) - see \ref invalidsettings-nochanges "below" for more detail.
 * \li Wait at least 10 generations (or longer!) to see whether the trees just need more time to generate a lucky combination of working nodes.
 *
 * The below subsections expand on some of these possible issues:
 *
 * \subsection invalidsettings-nochanges Invalid or incomplete EvolvingBehavior settings
 *
 * Double-check that your EvolutionControlActor and LifeCycle Actor have all their settings configured. Sometimes settings can get accidentally overwritten or deleted (such as when objects are copied or moved, etc.), so check that details of generated nodes, mutators, and other settings are all configured properly:
 *
 * \li Check that you have a large enough initial population that at least one agent is likely to show some action (probably ~30-50 agents should be enough - assuming your behavior trees can show some activity with only a few valid nodes).
 * \li Check that you are providing behavior tree nodes that will cause the movement/action you expect. The initial behavior tree, extra node library, or generated nodes need to have your desired actions in them. By saving the resulting trees from each generation, you could inspect them to see whether at least some of them include nodes that will perform your desired action.
 * \li Check that your settings for mutation and parent selection have high enough probability of changing the trees, so you should expect to see changes within a reasonable number of generations.
 * \li Check that your initial population is diverse enough (add more mutators or generations in the initial population settings if you don't see a variety of trees in the first generation).
 *
 * You can also search the logs for LogEvolvingBehavior to see if there are warnings or errors, but some settings misconfiguration issues may not show as errors.
 *
 * \subsection incorrect-tags Incorrect tags or not finding EvolutionControl Actors
 *
 * If you see that your agents are not running a behavior tree, it may be that your code is failing to find the correct EvolutionControlActor to retrieve the behavior trees from. Double-check that your EvolutionControlActors have the correct tags (for their specific population), name, or references set up in the scene for how you are finding them to retrieve the trees.
 *
 * \subsection incorrect-pathfinding Incorrect pathfinding settings
 *
 * As noted above, you may want to check using a non-evolving agent that your level has properly configured pathfinding (nav mesh, agents set up to use it, no unexpected colliders or other issues preventing navigation).
 *
 * The built-in Unreal AI debugger (https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/ArtificialIntelligence/AIDebugging/) may also help you find some issues with behavior trees and pathfinding.
 *
 * \subsection incomplete-setup Incomplete agent setup or fitness tracking
 *
 * If you are occasionally seeing movement, but not often or not consistently, it may be that you are not tracking fitness correctly (or your fitness functions are not encouraging the behavior you prefer). You should check that your code is sending fitness updates for all agents, that the weights and fitness functions are giving reasonable values (i.e., no single component of fitness is overwhelming the others, higher fitness is being found for agents showing good behavior, etc.). You can add debug logging of individual fitness component updates, or check the output \ref fitnesslogs "fitness log" files for total fitness scores.
 *
 * \subsection unlucky-start Unlucky random starting conditions
 *
 * If all your settings are correct, but none of the NPCs are moving, it could just be that you got unlucky initial behavior tree population where none of the trees happen to have useful configurations of nodes. If you wait a few generations, some trees may end up improving - but, because all the fitnesses will be similar, this may not happen quickly or at all. Sometimes, you may be better off restarting the experiment once (or even multiple times) to try a new set of random starting conditions.
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogEvolvingBehavior, Display, All);
DECLARE_LOG_CATEGORY_EXTERN(LogEvolvingBehaviorError, Error, All);
DECLARE_LOG_CATEGORY_EXTERN(LogEvolvingBehaviorWarning, Warning, All);

class FEvolvingBehaviorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle;
};
