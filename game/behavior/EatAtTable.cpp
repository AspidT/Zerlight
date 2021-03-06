//
// Created by tatiana on 26.06.2018.
//

#include "EatAtTable.h"
#include "Action.h"
#include "PathToChair.h"
#include "game/core/Character.h"
#include "game/core/Item.h"

namespace behavior
{
    EatAtTable::EatAtTable()
            :Sequence()
    {
        addNode(std::make_shared<PathToChair>());
        addNode(std::make_shared<Action>(&EatAtTable::wait,3.0f));
        addNode(std::make_shared<Action>(&EatAtTable::eatInComfort));
    }

    EatAtTable::~EatAtTable()
    {

    }

    TaskResult EatAtTable::wait(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        node->time-=dt;

        if (node->time>0.0f)
            return TaskResult::Running;

        node->time+=3.0f;

        return TaskResult::Success;
    }

    TaskResult EatAtTable::eatInComfort(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto list=character->heldItems();
        for (const auto& item : list)
        {
            if (item->effectAmount(game::ItemEffectType::Food)>0.0f)
                character->eatFoot(item);
        }

        character->setNeedGoal(nullptr);
        return TaskResult::Success;
    }
}