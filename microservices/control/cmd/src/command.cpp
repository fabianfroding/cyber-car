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
#include "envelopes.hpp"

int32_t main(int32_t argc, char **argv)
{
    /**Parse the arguments from the command line*/
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);

    if (0 != commandlineArguments.count("help"))
    {
        std::cerr << argv[0] << " is an example application for miniature vehicles (Kiwis) of DIT638 course." << std::endl;
        std::cerr << argv[0] << "[--carlos=<ID of carlos microservices>]" << std::endl;
        std::cerr << argv[0] << "[--cid=<ID of KIWI session>]" << std::endl;
        std::cerr << argv[0] << "[--turn=<turn angle>]" << std::endl;
        std::cerr << argv[0] << "[--verbose] print information" << std::endl;
        std::cerr << argv[0] << "[--debug] configure turns" << std::endl;
        std::cerr << argv[0] << "[--help]" << std::endl;
        std::cerr << "example:  " << argv[0] << "--cid=112 --carlos=113 --verbose" << std::endl;
        return -1;
    }
    const uint16_t CARLOS_SESSION{(commandlineArguments.count("carlos") != 0) ? static_cast<uint16_t>(std::stof(commandlineArguments["carlos"])) : static_cast<uint16_t>(113)};
    const uint16_t CID_SESSION{(commandlineArguments.count("cid") != 0) ? static_cast<uint16_t>(std::stof(commandlineArguments["cid"])) : static_cast<uint16_t>(112)};
    const float TURN{(commandlineArguments.count("turn") != 0) ? static_cast<float>(std::stof(commandlineArguments["turn"])) : static_cast<float>(0.2)};
    const float SP{(commandlineArguments.count("speed") != 0) ? static_cast<float>(std::stof(commandlineArguments["speed"])) : static_cast<float>(0.14)};
    const uint16_t DELAY{(commandlineArguments.count("delay") != 0) ? static_cast<uint16_t>(std::stof(commandlineArguments["delay"])) : static_cast<uint16_t>(1)};
    const bool DEBUG{commandlineArguments.count("debug") != 0};
    const uint16_t CID_SESSION{(commandlineArguments.count("cid") != 0) ? static_cast<uint16_t>(std::stof(commandlineArguments["cid"])) : static_cast<uint16_t>(112)};
    const bool VERBOSE{commandlineArguments.count("verbose") != 0};

    uint16_t dir = 12;

    if (VERBOSE)
    {
        std::cout << "starting up " << argv[0] << "..." << std::endl;
        std::cout << "turn: [" << TURN << "]" << std::endl;
        std::cout << "speed: [" << SP << "]" << std::endl;
        std::cout << "delay: [" << DELAY << "]" << std::endl;
        std::cout << "turn: [" << TURN << "]" << std::endl;
    }

    cluon::OD4Session carlos_session{CARLOS_SESSION};
    cluon::OD4Session kiwi_session{CID_SESSION};

    if (kiwi_session.isRunning())
    {
        std::cout << "session started..." << std::endl;

        bool SEMAPHORE = true;
        int16_t STAGE = 0;
        auto get_status = [&SEMAPHORE, &STAGE](cluon::data::Envelope &&envelope) {
            /** unpack message recieved*/
            auto msg = cluon::extractMessage<carlos::cmd::status>(std::move(envelope));
            /*store data*/
            SEMAPHORE = msg.semaphore();
            STAGE = msg.stage();
        };

        bool turn_west = true, turn_north = true, turn_east = true;
        auto turn_status = [VERBOSE, &turn_west, &turn_north, &turn_east](cluon::data::Envelope &&envelope) {
            /** unpack message recieved*/
            auto msg = cluon::extractMessage<carlos::cmd::turn_status>(std::move(envelope));
            /*store data*/
            turn_west = msg.turn_west();
            turn_north = msg.turn_north();
            turn_east = msg.turn_east();

            if (VERBOSE)
            {
                std::cout << "session started..." << std::endl;
            }
        };

        carlos_session.dataTrigger(carlos::cmd::status::ID(), get_status);
        carlos_session.dataTrigger(carlos::cmd::turn_status::ID(), turn_status);

        // float turn = TURN;
        // float speed = SP;
        // uint16_t delay_time = DELAY, message = 0;

        // while (kiwi_session.isRunning())
        // {
        //     if (SEMAPHORE)
        //     {
        //         if (DEBUG)
        //         {
        //             std::cout << "turn angle:" << std::endl;
        //             scanf("%f", &turn);
        //             std::cout << "speed:" << std::endl;
        //             scanf("%f", &speed);
        //             std::cout << "delay:" << std::endl;
        //             scanf("%hd", &delay_time);
        //             std::cout << "message:" << std::endl;
        //             scanf("%hd", &message);

        //             if ((-0.6 <= turn && turn <= 0.6) && speed >= 0 /*  && delay_time >= 0 */)
        //             {
        //                 //turn wheel
        //                 wheel.groundSteering(turn);
        //                 kiwi_session.send(wheel);

        //                 //speed
        //                 pedal.position(speed);
        //                 kiwi_session.send(pedal);

        //                 //delay
        //                 std::chrono::milliseconds timer(delay_time); // or whatever
        //                 std::this_thread::sleep_for(timer);

        //                 //stop vehicle
        //                 pedal.position(0);
        //                 kiwi_session.send(pedal);

        //                 //straighten wheel
        //                 wheel.groundSteering(0);
        //                 kiwi_session.send(wheel);

        //                 command.turn(message);
        //                 carlos_session.send(command);
        //             }
        //             else
        //             {
        //                 std::cout << "ERROR" << std::endl;
        //                 if (!(-0.6 <= turn && turn <= 0.6))
        //                 {
        //                     std::cout << "the turn angle should be between -0.6 and 0.6" << std::endl;
        //                 }
        //                 if (!(speed >= 0))
        //                 {
        //                     std::cout << "the speed should be greater than or equal to 0" << std::endl;
        //                 }
        //                 /* if (!(delay_time >= 0))
        //             {
        //                 std::cout << "the delay time should be greater than or equal to 0" << std::endl;
        //             } */
        //             }
        //             if (turn == 0 && speed == 0 && delay_time == 0)
        //             {
        //                 std::cout << "DEBUG session ended." << std::endl;
        //                 break;
        //             }
        //         }

        //         if (STAGE == 3)
        //         {
        //             const int16_t left = 1, right = 2, neutral = 0;
        //             int16_t userInp = -1;
        //             /*leaving intersection*/
        //             std::cout << "press: " << std::endl;
        //             std::cout << "[" << left << "] for left turn" << std::endl;
        //             std::cout << "[" << right << "] for right turn" << std::endl;
        //             std::cout << "[" << neutral << "] for neutral" << std::endl;
        //             //take in input
        //             scanf("%hd", &userInp);

        //             switch (userInp)
        //             {
        //             case left:
        //                 std::cout << "Carlos is turning [Left]" << std::endl;
        //                 break;
        //             case right:
        //                 std::cout << "Carlos is turning [Right]" << std::endl;
        //                 break;
        //             case neutral:
        //                 std::cout << "Carlos is [Straight]" << std::endl;
        //                 break;
        //             }
        //             //send data
        //             if (SEMAPHORE)
        //             {
        //                 kiwi_session.send(wheel);
        //             }
        //         }
        //     }
        //     else
        //     {
        //         std::cout << "RECIEVED -> SEMAPHORE [LOCKED]" << std::endl;
        //     }

        opendlv::proxy::GroundSteeringRequest wheel; //[car] groundSteering
        opendlv::proxy::PedalPositionRequest pedal;  //[car] pedal position

        while (car_session.isRunning())
        {
            std::cout << "Direction (9, 12, 3):" << std::endl;
            scanf("%d", &dir);

            switch (dir)
            {
            case 9:
                std::cout << "carlos turning left" << std::endl;
                //turn wheel
                wheel.groundSteering(0.14);
                car_session.send(wheel);

                //speed
                pedal.position(0.13);
                car_session.send(pedal);

                //delay
                std::chrono::milliseconds timer(3); // or whatever
                std::this_thread::sleep_for(timer);

                //stop vehicle
                pedal.position(0);
                car_session.send(pedal);

                //straighten wheel
                wheel.groundSteering(0);
                car_session.send(wheel);
                break;

            case 12:
                std::cout << "carlos turning right" << std::endl;
                //turn wheel
                wheel.groundSteering(0);
                car_session.send(wheel);

                //speed
                pedal.position(0.12);
                car_session.send(pedal);

                //delay
                std::chrono::milliseconds timer(3); // or whatever
                std::this_thread::sleep_for(timer);

                //stop vehicle
                pedal.position(0);
                car_session.send(pedal);

                //straighten wheel
                wheel.groundSteering(0);
                car_session.send(wheel);
                break;

            case 3:
                std::cout << "carlos turning right" << std::endl;
                //turn wheel
                wheel.groundSteering(-0.25);
                car_session.send(wheel);

                //speed
                pedal.position(0.13);
                car_session.send(pedal);

                //delay
                std::chrono::milliseconds timer(3); // or whatever
                std::this_thread::sleep_for(timer);

                //stop vehicle
                pedal.position(0);
                car_session.send(pedal);

                //straighten wheel
                wheel.groundSteering(0);
                car_session.send(wheel);
                break;
            case default:
                break;
            }
        }
    }
    else
    {
        std::cout << "Carlos Out. (Sarlos Session timed out.)" << std::endl;
    }
    return 0;
}
