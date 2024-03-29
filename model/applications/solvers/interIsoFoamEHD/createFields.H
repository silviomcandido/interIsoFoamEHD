#include "createRDeltaT.H"

Info<< "Reading field p_rgh\n" << endl;
volScalarField p_rgh
(
    IOobject
    (
        "p_rgh",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

#include "createPhi.H"


Info<< "Reading transportProperties\n" << endl;
immiscibleIncompressibleTwoPhaseEHDMixture mixture(U, phi);

volScalarField& alpha1(mixture.alpha1());
volScalarField& alpha2(mixture.alpha2());

const dimensionedScalar& rho1 = mixture.rho1();
const dimensionedScalar& rho2 = mixture.rho2();


Info<< "Reading electrical properties\n" << nl << endl;
//IOdictionary elPropDict
//(
//	IOobject
//	(
//		"electricalProperties",
//        	runTime.timeName(),
//        	mesh,
//        	IOobject::MUST_READ,
//        	IOobject::AUTO_WRITE
//	),
//	mesh
//);

// IR:  Read electrical properties from electricalProperties dictionary file
//dimensionedScalar eps1 = elPropDict.subDict("phase1").lookup("eps");
//dimensionedScalar eps2 = elPropDict.subDict("phase2").lookup("eps");
//dimensionedScalar sgm1 = elPropDict.subDict("phase1").lookup("sgm");
//dimensionedScalar sgm2 = elPropDict.subDict("phase2").lookup("sgm");

const dimensionedScalar& eps1 = mixture.eps1(); // ADD
const dimensionedScalar& eps2 = mixture.eps2(); // ADD
const dimensionedScalar& sgm1 = mixture.sgm1(); // ADD
const dimensionedScalar& sgm2 = mixture.sgm2(); // ADD


Info<<"Found: eps1=" << eps1 << ",eps2=" << eps2 << ",sgm1=" << sgm1 << ",sgm2=" << sgm2 << endl;


// Need to store rho for ddt(rho, U)
volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    alpha1*rho1 + alpha2*rho2
);
rho.oldTime();

volScalarField sgm
(
	IOobject
    (
	"sgm",
	runTime.timeName(),
	mesh,
	IOobject::READ_IF_PRESENT,
	IOobject::AUTO_WRITE
	),
	scalar(1) / (alpha1/sgm1 + alpha2/sgm2)
    );
    sgm.oldTime();

volScalarField eps						 
(
	IOobject
    (
	"eps",
	runTime.timeName(),
	mesh,
	IOobject::READ_IF_PRESENT,
	IOobject::AUTO_WRITE
	),
	scalar(1) / (alpha1/eps1 + alpha2/eps2)
    );
    eps.oldTime();				

// Mass flux
surfaceScalarField rhoPhi
(
    IOobject
    (
        "rhoPhi",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    fvc::interpolate(rho)*phi
);

Info<< "Reading field Electric Potential Ue\n" << endl;
    volScalarField Ue
    (
        IOobject
        (
            "Ue",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field electric density rhoE\n" << endl;
    volScalarField rhoE
    (
        IOobject
        (
            "rhoE",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    volTensorField MST
    (
        IOobject
        (
            "MST",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        eps*(fvc::grad(Ue)*fvc::grad(Ue) - scalar(0.5)*magSqr(fvc::grad(Ue))*I)
    );	

	// ADD
	volVectorField Ef
	(
		IOobject
		(
			"Ef",
			runTime.timeName(),
			mesh,
			IOobject::NO_READ,
			IOobject::AUTO_WRITE
		),
		-fvc::grad(Ue)
	);

	volVectorField Eft
	(
		IOobject
		(
			"Eft",
			runTime.timeName(),
			mesh,
			IOobject::NO_READ,
			IOobject::AUTO_WRITE
		),
		-scalar(0.5)*magSqr(Ef)*fvc::grad(eps)-fvc::laplacian(eps,Ue)*fvc::grad(Ue)
	);

// Construct incompressible turbulence model
autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, mixture)
);


#include "readGravitationalAcceleration.H"
#include "readhRef.H"
#include "gh.H"


volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    p_rgh + rho*gh
);

label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell
(
    p,
    p_rgh,
    pimple.dict(),
    pRefCell,
    pRefValue
);

if (p_rgh.needReference())
{
    p += dimensionedScalar
    (
        "p",
        p.dimensions(),
        pRefValue - getRefCellValue(p, pRefCell)
    );
    p_rgh = p - rho*gh;
}

mesh.setFluxRequired(p_rgh.name());
mesh.setFluxRequired(alpha1.name());

#include "createMRF.H"
#include "createFvOptions.H"

isoAdvection advector(alpha1, phi, U);
