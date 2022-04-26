      program p2p
      implicit none
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
      integer    ierr, nproc, myid, stat(MPI_STATUS_SIZE)
      integer vec(100000000), rvec(100000000)
      integer i, nl, ipto, ipfr, loop

c  initialize MPI, get nproc (# of procs) and myid (local proc. id)
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )

      do loop = 1 , 8
         nl = 10**loop
         do i = 1 , nl
            vec(i) =  myid
         end do
         write(6,10)nl,myid,vec(1)
 10      format('size',i10,' start value  ',i3,'>>',i2)
         ipfr = modulo(myid-1,nproc)
         ipto = modulo(myid+1,nproc)
         call MPI_SENDRECV( vec,nl,MPI_INTEGER,ipto,0,
     :                      rvec,nl,MPI_INTEGER,ipfr,0,
     :                       MPI_COMM_WORLD,stat,ierr )
         write(6,11)nl,myid,rvec(1)
 11      format('size',i10,' value after ring shift ',i3,'>>',i3)
      end do
      call MPI_FINALIZE( ierr )

      stop
      end



 
