// 2021/02/27 10:04:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Scene/Objects/Object_.h"
#include "Scene/Objects/Units/Accessories/Engine/Engine_.h"
#include "Scene/Objects/Units/Accessories/Engine/Logic_.h"


void EngineCalculator::Calculate(PhysicsParameters &physics, CommandEngine::E command, int count,
    EngineAlgorithm &algorithm)
{
    if (CalculateRotate(physics, command, algorithm))
    {
        CalculateMovement(physics, command, count, algorithm);
    }
}


bool EngineCalculator::CalculateRotate(PhysicsParameters &physics, CommandEngine::E command,
    EngineAlgorithm & algorithm)
{
    Vector3 position = physics.pos.GetWorld();

    Vector3 target = position;

    switch (command)
    {
    case CommandEngine::MoveToNorth:     target.x_ -= 1.0f;                    break;
    case CommandEngine::MoveToNorthEast: target.x_ -= 1.0f; target.z_ += 1.0f; break;
    case CommandEngine::MoveToEast:      target.z_ += 1.0f;                    break;
    case CommandEngine::MoveToEastSouth: target.z_ += 1.0f; target.x_ += 1.0f; break;
    case CommandEngine::MoveToSouth:     target.x_ += 1.0f;                    break;
    case CommandEngine::MoveToSouthWest: target.x_ += 1.0f; target.z_ -= 1.0f; break;
    case CommandEngine::MoveToWest:      target.z_ -= 1.0f;                    break;
    case CommandEngine::MoveToWestNorth: target.z_ -= 1.0f; target.x_ -= 1.0f; break;
    case CommandEngine::None:
        break;
    }

    if (TheTerrain->GetHeight(physics.pos.GetWorld().z_, physics.pos.GetWorld().x_) !=
        TheTerrain->GetHeight(target.z_, target.x_))
    {
        return false;
    }

    Step step(Step::Type::Rotate);

    step.endPos = target;

    algorithm.steps.Push(step);

    return true;
}


void EngineCalculator::CalculateMovement(PhysicsParameters &physics, CommandEngine::E command, int count,
    EngineAlgorithm &algorithm)
{
    Step step(Step::Type::Move);

    step.endPos = physics.pos.GetWorld();

    while (count-- > 0)
    {
        switch (command)
        {
        case CommandEngine::MoveToNorth:     step.endPos.x_ -= 1.0f;                         break;
        case CommandEngine::MoveToNorthEast: step.endPos.x_ -= 1.0f; step.endPos.z_ += 1.0f; break;
        case CommandEngine::MoveToEast:      step.endPos.z_ += 1.0f;                         break;
        case CommandEngine::MoveToEastSouth: step.endPos.z_ += 1.0f; step.endPos.x_ += 1.0f; break;
        case CommandEngine::MoveToSouth:     step.endPos.x_ += 1.0f;                         break;
        case CommandEngine::MoveToSouthWest: step.endPos.x_ += 1.0f; step.endPos.z_ -= 1.0f; break;
        case CommandEngine::MoveToWest:      step.endPos.z_ -= 1.0f;                         break;
        case CommandEngine::MoveToWestNorth: step.endPos.z_ -= 1.0f; step.endPos.x_ -= 1.0f; break;
        case CommandEngine::None:
            break;
        }

        if (TheTerrain->GetHeight(physics.pos.GetWorld().z_, physics.pos.GetWorld().x_) ==
            TheTerrain->GetHeight(step.endPos.z_, step.endPos.x_))
        {
            algorithm.steps.Push(step);
        }
        else
        {
            break;
        }
    }
}


EngineExecutor::Result EngineExecutor::Execute(PhysicsParameters &physics, float timeStep, EngineT &engine)
{
    if (engine.algorithm.IsFinished())
    {
        return EngineExecutor::Result::Finished;
    }

    switch (engine.algorithm.steps.Front().type)
    {
    case Step::Type::Move:      return ExecuteMovement(physics, timeStep, engine);   break;
    case Step::Type::Rotate:    return ExecuteRotate(physics, timeStep, engine);     break;
    case Step::Type::None:
        break;
    }

    return Result::Finished;
}


EngineExecutor::Result EngineExecutor::ExecuteMovement(PhysicsParameters &physics, float timeStep, EngineT &engine)
{
    Step &step = engine.algorithm.steps.Front();

    Vector3 currentPos = physics.pos.GetWorld();

    float dist = physics.max.SpeedMove() * timeStep;        // ����� ��������

    float delta = (step.endPos - currentPos).Length();      // �������� �� �������� �����

    if (dist >= delta)                                      // ���� ������� ������, ��� �����
    {
        physics.pos.SetWorld(step.endPos);

        return EngineExecutor::Result::Finished;            // �� ��������� ��������� ����
    }

    physics.pos.SetWorld(currentPos + physics.dir.GetWorldDir() * dist);

    return Result::Running;
}


//static void Calculate(char *name, PhysicsParameters &physics, Step &step)
//{
//    Vector3 position = physics.pos.GetWorld();
//    Vector3 dirToTarget = step.endPos - position;
//    dirToTarget.Normalize();                        // ����������� �� ����
//
//    Vector3 direction = physics.dir.GetWorldDir();          // ����������� ������ �����
//
//    float angleNeed = direction.Angle(dirToTarget); // �� ����� ���� ����� ��������� �����
//
//    LOGINFOF("");
//    LOGINFOF("%s : %f", name, angleNeed);
//}


Vector3 CalcualteDirToTarget(PhysicsParameters &physics, Step &step)
{
    Vector3 position = physics.pos.GetWorld();
    Vector3 dirToTarget = step.endPos - position;
    dirToTarget.Normalize();
    return dirToTarget;
}


//static float CalculateDelta(PhysicsParameters &physics, Step &step)
//{
//    Vector3 dirToTarget = CalcualteDirToTarget(physics, step);
//
//    Vector3 dir = physics.dir.GetWorldDir();
//
//    return (dir - dirToTarget).Length();
//}


EngineExecutor::Result EngineExecutor::ExecuteRotate(PhysicsParameters &physics, float dT, EngineT &engine)
{
    Vector3 &endPos = engine.algorithm.GetStep().endPos;            // � ���� ����� ����� �����������

    Vector3 dir = physics.dir.GetWorldDir();                        // ����������� �������� �����

    Vector3 dirToEndPos = endPos - physics.pos.GetWorld();
    dirToEndPos.Normalize();                                        // ����������� �� ����

    float angleNeed = dir.Angle(dirToEndPos);

    float angleCan = physics.max.SpeedRotate() * dT;

    Vector3 axixRotate = dir.CrossProduct(dirToEndPos);

    if (angleCan < angleNeed)                       // ���� �� ���� ����� �� ��������� ��������� �����������
    {
        Quaternion delta(angleCan, axixRotate);
        physics.rot.ChangeWorld(delta);
        return Result::Running;
    }

    physics.rot.ChangeWorld(Quaternion(angleNeed, axixRotate));

    return Result::Finished;
}
