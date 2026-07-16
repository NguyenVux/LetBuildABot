#pragma once

class Player;
class State;
class Instruction 
{
protected:
    Instruction* m_next; 
public:
    Instruction(): m_next(nullptr) {}
    Instruction* Next(){ return m_next;}
    virtual void Setup(State& state) = 0;
    virtual void Execute(State& state) = 0; 
    virtual bool isDone(State& state) const = 0;
};


class MoveInstruction : public Instruction
{
public:
    Vector2 m_start;
    Vector2 m_end;
public:
    void SetNext(Instruction* next) {m_next = next;}
    virtual void Setup(State& state) override;
    virtual void Execute(State& state) override;
    virtual bool isDone(State& state) const override;
};

class DelayInstruction : public Instruction
{
public:
    float m_duration;
    float m_elapsed;
public:
    DelayInstruction(float duration) : m_duration(duration), m_elapsed(0.0f) {}
    void SetNext(Instruction* next) {m_next = next;}
    virtual void Setup(State& state) override;
    virtual void Execute(State& state) override;
    virtual bool isDone(State& state) const override;
};