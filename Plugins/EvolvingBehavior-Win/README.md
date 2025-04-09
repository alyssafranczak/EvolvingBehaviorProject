# Evolving Behavior

## An AI-assisted tool to experimentally refine behavior trees through interactive evolution. Main site: https://evolvingbehavior.npc.codes/

### Authors: Nathan Partlan (NPCDev), Jim Howe, Luis Soto, Sarthak Shrivastava, Alex Grundwerg, Isha Srivastava

### Collaboration and Advice: Magy Seif El-Nasr, Stacy Marsella, Erica Kleinman, Sabbir Ahmad, Muhammad Ali, Zheng Fang

### Special Thanks: Tom Looman

## Summary

This is a plugin and tool for use with Unreal® Engine 4, currently designed for use with version 4.27. It works on top of the existing Unreal Engine 4 behavior tree technology to evolve new behavior.

You will need to provide a basic behavior tree, and specify various additional nodes and properties that could be generated and added to the tree. Then, you will need to specify how your behavior trees should be tested: requesting trees to be generated for agents, how their fitness should be calculated, and how long each experiment should last.

By running the experiment and interacting with the generated agents to test their fitness, you will then help to guide the evolutionary process towards behavior that fits your goals.

## More Information

For documentation and more information about the project, see:

https://evolvingbehavior.npc.codes/

## License

The Evolving Behavior plugin is free and open source under the MIT License, which can be used in conjunction with code licensed under the Unreal Engine EULA. Note that, to use this plugin in Unreal Engine 4, you will need to have your own license to use the Unreal Engine 4 software itself.

**IMPORTANT NOTE**: Although the MIT License legally permits anyone to use this software, we, the authors, strongly condemn any use of this software for military, intelligence agency, and law enforcement purposes. We also condemn any use of this software for other purposes that violate human rights, such as any use that violates the United Nations Universal Declaration of Human Rights (https://www.un.org/en/universal-declaration-human-rights/). We consider such uses, as well as use in conjunction with unreasonable profit-seeking, unfair labor practices, privacy violations, and other harms to users to be in violation of our permission to use this software, despite the license terms. We will work to stop any such uses of this software of which we become aware. Do not use this software for such purposes.

The Evolving Behavior plugin uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc. All rights reserved.

For full licensing information, see LICENSE.md

## Installing

Copy this folder to your "Plugins" directory in your Unreal Engine 4 project folder.

Alternatively, to install as an Engine plugin for all projects, copy to the "Plugins" directory inside your Unreal Engine 4 engine folder.

You may need to recompile your project and/or restart Unreal Engine 4.

For more information, see: https://docs.unrealengine.com/en-US/Programming/Plugins/index.html

## Developing

To make changes to the code and use the modified version in your project:

- Make a fork of this project's repository.

- Clone your fork of this project as a git submodule inside your project's Plugins directory.

- When you edit the code, be sure to recompile and hot reload in Unreal Engine 4.

- If you would like to contribute your changes back to the original project, please submit a merge request or patch file to the main git repository.

