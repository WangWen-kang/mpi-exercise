      program hello
      implicit none
      include 'mpif.h'
      integer nproc, myid, ierr, nprime, ntest, nloc, nr, nd ,i, ip
      integer numdiv(0:128)
c---------------------------------------------------------- 
c        start MPI
c----------------------------------------------------------
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
      nprime = 118845 
      ntest = sqrt(real(nprime))+1
      nloc = (ntest+nproc-1)/nproc
      nd = 0
      do i = max(3,myid*nloc+1),min(ntest,(myid+1)*nloc)
         nr = mod(nprime,i)
         if (nr.eq.0) then
            write(6,*)myid,' : ', i
            nd = nd+1
         end if
      end do
      if (myid.gt.0) then
         call MPI_Ssend(nd, 1, MPI_INTEGER, 0, 0 , MPI_COMM_WORLD, ierr)
      else
         numdiv(ip)= nd
         do ip = 1 , nproc-1
            call MPI_RECV(numdiv(ip), 1, MPI_INTEGER, ip, 0, 
     :              MPI_COMM_WORLD, MPI_STATUS_IGNORE, ierr)
         end do
         write(6,*)'number of divisors found in different processes:'
         write(6,19)(numdiv(ip),ip=0,nproc-1)
 19      format(16i3) 
      end if
      call MPI_FINALIZE (ierr)
      
      stop
      end






















