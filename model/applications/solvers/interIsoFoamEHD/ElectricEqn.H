            surfaceScalarField rhoEFlux = -fvc::interpolate(sgm)*mesh.magSf()*fvc::snGrad(Ue);
            fvScalarMatrix rhoEEqn
            (
                fvm::ddt(rhoE)
              + fvm::div(phi, rhoE)
              + fvc::div(rhoEFlux)
            );
            rhoEEqn.solve();
            
      	    // solve
            fvScalarMatrix UeEqn
            (
                fvm::laplacian(eps,Ue) == -rhoE
            );
            UeEqn.solve();
			
			Ef = -fvc::grad(Ue); // electric field intensity
			Eft = rhoE*Ef -scalar(0.5)*magSqr(Ef)*fvc::grad(eps); // total electric force

