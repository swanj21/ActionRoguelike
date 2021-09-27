# ActionRoguelike

## Assignments
### Assignment 2
+ Problem statement: Projectiles don't hit where the player expects(under the crosshair)
  + Projectile is spawned in player hand which can be left or right based on orientation
  + Camera perspective is deceiving, aiming at something really close will miss target due to camera alignment with character
  + Solution: Line trace from Camera to World and find desired impact location
    + Re-calculate new projectile spawn Rotation by using the spawn location(hand pos) and impact location(line trace result)
    + If nothing was hit, use the trace end vector as the desired target
    + Make sure to trace against multiple object types(dynamic, static)

**Complete**

------------
+ Create "Blackhole" ability
  + Projectile class spawned via key input
  + RadialForceComponent but using continuous negative force to pull in actors
    + Ignore 'Pawn' collision object type(player must remain unaffected by pull)
  + Extra SphereComponent to Destroy() actors on overlap(black hole)
    + Must only destroy 'simulating' actors
  + Particle component for the black hole VFX
  + Destroy itself after ~5 seconds(match particle system duration)
  + Made in blueprints(except for input binding and spawning)
+ Create dash/teleport projectile ability
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

### Assignment 1
+ Setup project in Github - <b>COMPLETE</b>
+ Character w/ movement & look input - <b>COMPLETE</b>
+ Implement the Magic Projectile - <b>COMPLETE</b>
+ Bind Jumping via BindAction() - <b>COMPLETE</b>
  + _Personal challenge: Find the right jump z velocity that feels good when controlling the player_ - <b>COMPLETE</b>
+ Explosive barrel implemented in c++ - <b>COMPLETE</b>
