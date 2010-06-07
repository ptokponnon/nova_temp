/*
 * Generic Space
 *
 * Author: Udo Steinberg <udo@hypervisor.org>
 * TU Dresden, Operating Systems Group
 *
 * This file is part of the NOVA microhypervisor.
 *
 * NOVA is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NOVA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License version 2 for more details.
 */

#pragma once

#include "compiler.h"
#include "lock_guard.h"
#include "mdb.h"
#include "spinlock.h"

class Space
{
    private:
        Spinlock    lock;
        Avl *       tree;

    public:
        Space() : tree (0) {}

        Mdb *lookup_node (mword idx)
        {
            Lock_guard <Spinlock> guard (lock);
            return Mdb::lookup (tree, idx);
        }

        bool insert_node (Mdb *node)
        {
            Lock_guard <Spinlock> guard (lock);
            return Mdb::insert (&tree, node);
        }

        bool remove_node (Mdb *node)
        {
            Lock_guard <Spinlock> guard (lock);
            return Mdb::remove (&tree, node);
        }
};