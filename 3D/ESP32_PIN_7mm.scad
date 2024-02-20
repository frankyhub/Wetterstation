//Platinen Pin ESP32 UNO Wetterstation
//4x

$fn=60;
Rgr = 3; //Radius PIN 
R1kl = 0; //Radius 1 Stift
R2kl = 0; //Radius 2 Stift
Sh = 7;       //Sockelhöhe


cylinder(h = Sh, r1=Rgr , r2=Rgr , center = true);
translate(v = [0, 0, Sh/2])
cylinder(h = 3, r1 = R1kl, r2 = R2kl, center = true);