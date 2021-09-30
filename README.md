# ActionRoguelike

## Assignments
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

----
**Player Character**  
_React to damage received_
+ Add 'hit flash' material nodes to existing player material (M_CharacterSimple using 'Material Function')
+ Trigger material via OnHealthChanged when 'Damaged' only
+ Must have tweakable 'Color' and 'Speed' parameters(can have others as well)

----
**AttributeComponent**  
_HealthMax attribute_
+ Add HealthMax attribute
+ Replace 'ClassDefaults::Health' node in Health Widget with new HealthMax attribute
+ Clamp Health b/w 0 and HealthMax via ApplyHealthChanges (See FMath)

----
**UI**  
_Animation_
+ Add 'Damage amount' to the new damage widget (Expose on spawn to set value)
+ Add animation to widget when added to viewport (color, scale, etc...) (open ended)

----
**Health Potion power up**  
_Healing pickup_
+ React to the Interact() function on the GameplayInterface
+ Once triggered, de-activate for 10s(invisible, no interactions)
  + Reactivates once time elapsed(show mesh, can be triggered)
+ Ignores interacting pawn if already at max health

----
**Magic projectile**  
_Enhancing the projectile_
+ Play 'attached' particle effect in player's hand when attacking
  + UGameplayStatics::SpawnEmitterAttached, P_Gideon_Primary_MuzzleFlash
+ World camera shake on projectile impact
  + UGameplayStatics::PlayWorldCameraShake - Create a new shake asset

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
