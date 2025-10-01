/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     commands                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "../include/commands.hpp"

std::vector<uint8_t> encodeMoveResquest(const MoveRequest& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x23);

    buffer.emplace_back(pos.direction & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeShootResquest(const ShootRequest& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x25);

    return buffer;
}

std::vector<uint8_t> encodePickupItemResquest(const PickupItemResquest& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x40);

    return buffer;
}
