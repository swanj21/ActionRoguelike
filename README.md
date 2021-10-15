# ActionRoguelike

## Assignments

----

### Assignment 6
**New Attributes & Buffs**  
_Add new buffs_

New 'Rage' attribute
* Player receiving damage adds to its Rage value (RCharacter::OnHealthChanged)
* Blackhole action costs x Rage to activate
* Display current Rage on screen
  * UMG UI for Rage amount
* RageMax to clamp and for UI display with a bar

_COMPLETE_

'Thorns' buff
* ActionEffect in c++ that deals a fraction of damage received back to attacker
  * If you take 20 damage, reflect a % of that damage back to whoever initiated the attack
  * Bind to OnHealthChanged of owner in StartAction(and don't forget to unbind [ .RemoveDynamic ] in StopAction)
* Round the reflected damage to the nearest Integer
* Infinite duration (Duration=0.0)
  * Also no periodic effect is needed
* Don't reflect damage caused to yourself (Instigator == self ? Don't do it)

Player spotted UMG widget
* Derived from RWorldUserWidget
* Show ! for a moment when minion sees player
* Only show if Player is not already the TargetActor
* Animate for nice pop-in

New powerup to grant Actions on Interact
* Only interactable if user does not have that Action yet
* Grant Dash on Interact(remove Dash from default set of Player's Actions)

----
### Assignment 5
**Credit System**  
_Create a credit system_
+ Killing minions **gives** credits
+ Picking up a coin **gives** credits
  + Coin **MUST** use shared base class w/ health potion and use activate, hide, respawn functionality
+ Using a health potion **costs** credits
+ Credits widget hooked up to show current credits
+ Credits must remain even if player dies(with a penalty) 
  + New PlayerState class
+ **BONUS**
  + Randomly spawn coin and health potions throughout a simple level at game start using EQS

Note: System should be designed for ease of use in c++ and BP 

_COMPLETE_

----
### Assignment 4
**New AI Behavior**  
_Flee and heal when low health_
+ Create new behavior, flee/hide when low health(determine some threshold) and heal
  + Service should check periodically if low health
  + EQS should find hidden position away from player & close to AI
  + New BT Task to heal back to max hitpoints
  + Can only run once every 60(determine proper number) seconds
  + Fight as normal after healed

_COMPLETE_

----
### Assignment 3
**Update Magic Projectile**  
_Audio and visual polish_  
+ Add audio playback
  + Add a looped 'flight' sound (UAudioComponent)
  + Add an impact sound (UGameplayStatics::PlaySoundAtLocation)
+ Any logic in BP, move to C++

_COMPLETE_

**Bug to resolve: Impact sound does not play when projectile is destroyed outside of the 
OnActorOverlap() function.**

_BUG COMPLETE_

----
**Player Character**  
_React to damage received_
+ Add 'hit flash' material nodes to existing player material (M_CharacterSimple using 'Material Function')
+ Trigger material via OnHealthChanged when 'Damaged' only
+ Must have tweakable 'Color' and 'Speed' parameters(can have others as well)

_COMPLETE_

----
**AttributeComponent**  
_HealthMax attribute_
+ Add HealthMax attribute
+ Replace 'ClassDefaults::Health' node in Health Widget with new HealthMax attribute
+ Clamp Health b/w 0 and HealthMax via ApplyHealthChanges (See FMath)

_COMPLETE_

----
**UI**  
_Animation_
+ Add 'Damage amount' to the new damage widget (Expose on spawn to set value)
+ Add animation to widget when added to viewport (color, scale, etc...) (open ended)

_COMPLETE_

----
**Health Potion power up**  
_Healing pickup_
+ React to the Interact() function on the GameplayInterface
+ Once triggered, de-activate for 10s(invisible, no interactions)
  + Reactivates once time elapsed(show mesh, can be triggered)
+ Ignores interacting pawn if already at max health

_COMPLETE_

----
**Magic projectile**  
_Enhancing the projectile_
+ Play 'attached' particle effect in player's hand when attacking
  + UGameplayStatics::SpawnEmitterAttached, P_Gideon_Primary_MuzzleFlash
+ World camera shake on projectile impact
  + UGameplayStatics::PlayWorldCameraShake - Create a new shake asset

_COMPLETE_

----
### Assignment 2
**Projectiles don't hit where the player expects(under the crosshair)**
+ Projectile is spawned in player hand which can be left or right based on orientation
+ Camera perspective is deceiving, aiming at something really close will miss target due to camera alignment with character
+ Solution: Line trace from Camera to World and find desired impact location
  + Re-calculate new projectile spawn Rotation by using the spawn location(hand pos) and impact location(line trace result)
  + If nothing was hit, use the trace end vector as the desired target
  + Make sure to trace against multiple object types(dynamic, static)

_Complete_

------------
**Create "Blackhole" ability**
+ Projectile class spawned via key input
+ RadialForceComponent but using continuous negative force to pull in actors
  + Ignore 'Pawn' collision object type(player must remain unaffected by pull)
+ Extra SphereComponent to Destroy() actors on overlap(black hole)
  + Must only destroy 'simulating' actors
+ Particle component for the black hole VFX
+ Destroy itself after ~5 seconds(match particle system duration)
+ Made in blueprints(except for input binding and spawning)

_Complete_

-------------
**Create dash/teleport projectile ability**
+ Projectile class spawned via input key
+ ParticleComponent to show projectile
+ Explodes after .2 seconds(timer)
  + Play particle effect at point of detonation
+ Wait another .2 seconds(timer) and teleport PlayerCharacter(instigator of projectile)
  + Let detonation effect play before teleport
  + Make sure you stop the projectile movement while you wait
  + Look back at instigator we used for ignoring collisions and re-apply here for teleportation
+ On hit with world
  + Immediately stop movement and execute same behavior(explode & teleport)
+ Must be made in c++, NOT blueprint
Note: Consider making a shared base-class(due to the 3 projectile based abilities)

_Complete_

-------------

### Assignment 1
+ Setup project in Github - <b>COMPLETE</b>
+ Character w/ movement & look input - <b>COMPLETE</b>
+ Implement the Magic Projectile - <b>COMPLETE</b>
+ Bind Jumping via BindAction() - <b>COMPLETE</b>
  + _Personal challenge: Find the right jump z velocity that feels good when controlling the player_ - <b>COMPLETE</b>
+ Explosive barrel implemented in c++ - <b>COMPLETE</b>
