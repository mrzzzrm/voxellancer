#pragma once

#include <memory>


/**
 * This algorithm takes the task of placing or removing
 * Equipment into or from a EquipmentSlot
 */
template<typename EquipmentSlotType, typename EquipmentType>
class EquipmentChanger final {
public:
    EquipmentChanger(EquipmentSlotType& slot, std::shared_ptr<EquipmentType>& target, const std::shared_ptr<EquipmentType>& newEquipment);

    void change();


protected:
    EquipmentSlotType& m_slot;
    std::shared_ptr<EquipmentType>& m_target;
    const std::shared_ptr<EquipmentType>& m_newEquipment;

    void setSlot(EquipmentType& equipment, EquipmentSlotType* slot);
};


#include "equipmentchanger.inl"
