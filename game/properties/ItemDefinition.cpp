//
// Created by tatiana on 09.06.2018.
//

#include "ItemDefinition.h"
#include <algorithm>

namespace properties
{
    ItemDefinition::ItemDefinition()
    {
        CombatRatingModifier=0.0f;
        EquippedJobPenalty=0.0f;
        EquippedMovePenalty=0.0f;
        WeaponSize=0.0f;
        TwoHanded=false;
        ItemWeaponDef= nullptr;
    }

    ItemDefinition::~ItemDefinition()
    {

    }

    void ItemDefinition::addEffect(game::ItemEffectType a_effect, float a_amount)
    {
        Effects.push_back(ItemEffect(a_effect,a_amount));
    }

    bool ItemDefinition::hasEffect(game::ItemEffectType effectType) const
    {
        auto iter=std::find_if(Effects.begin(),Effects.end(),[&effectType](ItemEffect const& value)
        {
            return (effectType==value.Effect);
        });

        return (iter!=Effects.end());
    }

    float ItemDefinition::effectAmount(game::ItemEffectType type) const
    {
        auto iter=std::find_if(Effects.begin(),Effects.end(),[&type](ItemEffect const& value)
        {
            return (type==value.Effect);
        });

        if (iter!=Effects.end())
            return iter->Amount;
        else
            return 0.0f;
    }

    float ItemDefinition::effectModifier(game::ItemQuality quality)
    {
        switch (quality)
        {
            case game::ItemQuality::Poor:
                return 0.9f;
            case game::ItemQuality::Average:
                return 1.0f;
            case game::ItemQuality::Fine:
                return 1.5f;
            case game::ItemQuality::Superior:
                return 2.0f;
            case game::ItemQuality::Masterful:
                return 3.0f;
            case game::ItemQuality::Legendary:
                return 5.0f;
            default:
                return 1.0f;
        }
    }

    float ItemDefinition::effectModifierForFoodAndDrink(game::ItemQuality quality)
    {
        switch (quality)
        {
            case game::ItemQuality::Poor:
                return 0.9f;
            case game::ItemQuality::Average:
                return 1.0f;
            case game::ItemQuality::Fine:
                return 2.0f;
            case game::ItemQuality::Superior:
                return 3.0f;
            case game::ItemQuality::Masterful:
                return 5.0f;
            case game::ItemQuality::Legendary:
                return 10.0f;
            default:
                return 1.0f;
        }
    }

    std::string ItemDefinition::SpriteID::getSpriteIDByMaterialID(const std::string& materialID) const
    {
        const auto iter=std::find_if(spriteIDByMaterialID.begin(),spriteIDByMaterialID.end(),
                [&materialID](std::pair<std::string,std::string> const& item)
        {
            return item.first==materialID;
        });

        if (iter!=spriteIDByMaterialID.end())
            return (*iter).second;
        else
            return "";
    }
}