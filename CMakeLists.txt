cmake_minimum_required(VERSION 3.12.2)

project(untitled13)
set(CMAKE_CXX_FLAGS  "-pthread")
set(CMAKE_CXX_STANDARD 14)

add_executable(untitled13 main.cpp Command.h OpenServerCommand.cpp DataReaderServer.cpp ConnectCommand.cpp DataReaderClient.cpp DefineVarCommand.cpp LoopCommand.cpp Expression.h Number.cpp BinaryExpression.cpp Plus.cpp Minus.cpp Div.cpp Mul.cpp FileReader.cpp PrintCommand.cpp ConditionParser.cpp IfCommand.cpp CommandExpression.cpp Smaller.cpp Bigger.cpp BiggerOrEqual.cpp SmallerOrEqual.cpp Equal.cpp NotEqual.cpp ShuntingYardParser.cpp Thread.h SleepCommand.cpp BufferParser.cpp GlobalBindMap.h GlobalBindMap.cpp)