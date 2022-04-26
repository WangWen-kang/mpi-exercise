      program gather_vec
      implicit none
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
      integer    ierr, np, myid, status(MPI_STATUS_SIZE)
      integer    nmaxlcl, nmaxglb, npmx
      parameter (nmaxlcl = 2**20, nmaxglb = 2**20, npmx = 64 )
      real*8 v_lcl(0:nmaxlcl), v_glb(0:nmaxglb)
      integer nglb, i, ip, counts(0:npmx), dspls(0:npmx)

c  initialize MPI, get nproc (# of procs) and myid (local proc. id)
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, np, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
      if (np.gt.npmx) then
         write(6,*)'np =',np,'larger than npmx =',npmx
         stop
      end if

! Define the partition vector dspls(0:np) for a global vector of size nglb
! and the size vector counts(0:np-1) with the sizes of local vectors 
      dspls(0) = 0
      do ip = 1 , np
         dspls(ip) = dspls(ip-1)+ 3+(ip-1)
         counts(ip-1) = dspls(ip) - dspls(ip-1) 
      end do
      nglb = dspls(np)

! Initialize the local vectors (such that v_glb(i) = i):
      do i = 0 , counts(myid)-1
         v_lcl(i) = dspls(myid)+i 
      end do
      write(6,100)myid,(v_lcl(i),i=0,counts(myid)-1)
 100  format('v_lcl auf',i3,'>>',100f6.0)
      
! Gather local vectors v_lcl to their places in v_glb of process 0
! Syntax:
! MPI_GATHERV( sendbuf, sendcount, sendtype, recvbuf, counts,
!             dspls, recvtype, root,comm)
      call MPI_GATHERV( v_lcl, counts(myid), MPI_DOUBLE_PRECISION,
     :                  v_glb, counts, dspls, MPI_DOUBLE_PRECISION,
     :                  0, MPI_COMM_WORLD, ierr)

      call MPI_BCAST( v_glb, nglb, MPI_DOUBLE_PRECISION,
     :                0, MPI_COMM_WORLD, ierr)

! Alternativly combine gather and broadcast with ALLGATHERV
! Syntax:
! MPI_ALLGATHERV( sendbuf, sendcount, sendtype, recvbuf,
!                recvcounts, displs,recvtype, comm)
!      call MPI_ALLGATHERV( v_lcl, recvcts(myid),MPI_DOUBLE_PRECISION,.. 

      write(6,101) myid, (v_glb(i),i = 0 , nglb-1)
 101  format('v_glb auf',i3,'>>',100f6.0)
      call MPI_FINALIZE( ierr )

      stop
      end



 
