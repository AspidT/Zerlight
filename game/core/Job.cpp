//
// Created by tatiana.polozova on 26.03.2018.
//

#include "Job.h"
#include "Item.h"
#include "Pathfinder.h"
#include "ResourcePile.h"
#include "Character.h"
#include <algorithm>

namespace game
{
    Job::Job(const map::vector3& position)
    {
        mPosition=position;

        mRequiredCharacter=nullptr;
        mClaimedBy=nullptr;
        mAdjacentComponents=false;
    }

    Job::~Job()
    {

    }

    const std::shared_ptr<Character>& Job::requiredCharacter()
    {
        return mRequiredCharacter;
    }

    bool Job::isClaimed(std::shared_ptr<Item> item) const
    {
        auto iter=std::find_if(std::begin(mClaimedComponents),std::end(mClaimedComponents),[&item](std::shared_ptr<Item> const& value)
        {
            return value->ID()==item->ID();
        });

        return (iter!=std::end(mClaimedComponents));
    }

    bool Job::componentInPosition(std::shared_ptr<Item> item) const
    {
        if ((item->position()!=mPosition) && (!mAdjacentComponents || Pathfinder::isAdjacent(item->position(),mPosition)))
            return false;

        if (item->parent()!= nullptr)
            return (std::dynamic_pointer_cast<ResourcePile>(item->parent())!=nullptr);

        if (mClaimedBy==nullptr)
            return false;

        return !mClaimedBy->hasItem(item);
    }

    std::shared_ptr<Item> Job::nextComponent() const
    {
        for (auto item : mClaimedComponents)
        {
            if (!this->componentInPosition(item))
                return item;
        }

        return nullptr;
    }

}