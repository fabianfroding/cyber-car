/**
 * Copyright (C) 2019 Carlos, the car
*/

#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cluon-complete.hpp"
#include "messages.hpp"

int32_t main(int32_t argc, char **argv)
{
    /**Parse the arguments from the command line*/
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);

    if (0 != commandlineArguments.count("help"))
    {
        std::cerr << argv[0] << " is an example application for miniature vehicles (Kiwis) of DIT638 course." << std::endl;
        std::cerr << "Usage:" << argv[0] << " --cid=<CID of your OD4Session>" << std::endl;
        std::cerr << argv[0] << "[--carlos=<ID of carlos microservices>]" << std::endl;
        std::cerr << argv[0] << "[--freq=<frequency>]" << std::endl;
        std::cerr << argv[0] << "[--verbose" << std::endl;
        std::cerr << argv[0] << "[--extra_v" << std::endl;
        std::cerr << argv[0] << "[--help]" << std::endl;
        std::cerr << "example:  " << argv[0] << " --cid=112 --carlos=646" << std::endl;
        return -1;
    }
    const uint16_t CARLOS_SESSION{(commandlineArguments.count("carlos") != 0) ? static_cast<uint16_t>(std::stof(commandlineArguments["carlos"])) : static_cast<uint16_t>(113)};
    const uint16_t CID_SESSION{(commandlineArguments.count("cid") != 0) ? static_cast<uint16_t>(std::stof(commandlineArguments["cid"])) : static_cast<uint16_t>(112)};
    const float FREQ{(commandlineArguments.count("freq") != 0) ? static_cast<float>(std::stof(commandlineArguments["freq"])) : static_cast<float>(30)};
    const bool VERBOSE{commandlineArguments.count("verbose") != 0};
    const bool EXTRA_VERBOSE{commandlineArguments.count("extra_v") != 0};

    if (VERBOSE || EXTRA_VERBOSE)
    {
        std::cout << "starting up " << argv[0] << "..." << std::endl;
    }

    /**
     * create a od4session object that will allow all microservices
     * with the same secret number to send and recieve messages from
     * one another
    */
    cluon::OD4Session carlos_session{CARLOS_SESSION}; //needed to send messages to carlos session
    cluon::OD4Session car_session{CID_SESSION};       //needed to send messages to car

    if (car_session.isRunning())
    {
        if (VERBOSE || EXTRA_VERBOSE)
        {
            std::cout << "[delegator] micro-service started..." << std::endl;
        }

        /*global variables*/
        float speed = 0;
        opendlv::proxy::PedalPositionRequest pedalReq;

        /* prepare messages to recieve from carlos session */
        auto distanceTrigger = [VERBOSE, &pedalReq, &speed](cluon::data::Envelope &&envelope) {
            /** unpack message recieved*/
            auto msg = cluon::extractMessage<carlos::acc>(std::move(envelope));
            /*store speed value from acc microservice*/
            speed = msg.speed();
            /*store speed in car object*/
            pedalReq.position(speed);

            if (VERBOSE)
            {
                std::cout << "set speed to [" << speed << "]" << std::endl;
            }
        };

        /*registers callbacks*/
        carlos_session.dataTrigger(carlos::acc::ID(), distanceTrigger);

        /*prepare time related functions*/
        auto sendPedalRequest{[VERBOSE, &car_session, &pedalReq]() -> bool {
            /*send pedal req object to car*/
            car_session.send(pedalReq);

            if (VERBOSE)
            {
                std::cout << "set speed to [" << pedalReq.position() << "]" << std::endl;
            }
            return true;
        }};

        /*register time triggers*/
        car_session.timeTrigger(FREQ, sendPedalRequest);

        while (car_session.isRunning())
        {
            /* just run this microservice until ist crashes */
        }
    }
    else
    {
        std::cout << "Carlos Out. (OD4Session timed out.)" << std::endl;
    }
    return 0;
}