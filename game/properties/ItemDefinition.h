//
// Created by tatiana on 09.06.2018.
//

#ifndef ZERL_ITEMDEFINITION_H
#define ZERL_ITEMDEFINITION_H

#include "game/properties/ItemSettings.h"

#include <vector>
#include <string>
#include <memory>

namespace properties {
    struct WeaponDef;

    struct ItemDefinition {
    public:
        ItemDefinition();
        ~ItemDefinition();

        std::string ID;
        std::string Name;
        std::string Prefix;
        std::string Suffix;

        float EquippedJobPenalty;
        float EquippedMovePenalty;

        std::vector<ItemEffect> Effects;
        void addEffect(game::ItemEffectType a_effect, float a_amount);

        bool hasEffect(game::ItemEffectType effectType) const;

        float effectAmount(game::ItemEffectType type) const;
        static float effectModifier(game::ItemQuality quality);
        static float effectModifierForFoodAndDrink(game::ItemQuality quality);

        std::shared_ptr<WeaponDef> ItemWeaponDef;
        game::EquipmentType EquipSlot;
        bool TwoHanded;
        float CombatRatingModifier;
        float WeaponSize;

        float Thickness;

        struct SpriteID
        {
            std::string spriteID;
            bool useMaterial;
            std::vector<std::pair<std::string,std::string>> spriteIDByMaterialID;

            std::string getSpriteIDByMaterialID(const std::string& materialID) const;
        };

        std::vector<SpriteID> Sprites;
    };
}

#endif //ZERL_ITEMDEFINITION_H
