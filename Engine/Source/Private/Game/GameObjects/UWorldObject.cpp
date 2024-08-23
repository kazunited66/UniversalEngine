#include "Game/GameObjects/UWorldObject.h"
#include "Graphics/UGraphicsEngine.h"
#include "Graphics/UModel.h"

TWeak<UModel> UWorldObject::ImportModel(const UString& path)
{
    if (const auto& modelRef = UGameEngine::GetGameEngine()->GetGraphics()->ImportModel(path)) {
        m_objectModels.push_back(modelRef);
        modelRef->GetTransform() = m_transform;
        return modelRef; 
    }
    return {};
}

void UWorldObject::OnPostTick(float deltaTime)
{
    UObject::OnPostTick(deltaTime); 

    //all models will follow the world object 
    for (const auto& modelRef : m_objectModels) {
        modelRef->GetTransform() = GetTransform() ; 
    }
}
