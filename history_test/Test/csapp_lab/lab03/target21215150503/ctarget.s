
ctarget:     file format elf64-x86-64


Disassembly of section .init:

0000000000400c48 <_init>:
  400c48:	48 83 ec 08          	sub    $0x8,%rsp
  400c4c:	48 8b 05 a5 43 20 00 	mov    0x2043a5(%rip),%rax        # 604ff8 <__gmon_start__>
  400c53:	48 85 c0             	test   %rax,%rax
  400c56:	74 02                	je     400c5a <_init+0x12>
  400c58:	ff d0                	callq  *%rax
  400c5a:	48 83 c4 08          	add    $0x8,%rsp
  400c5e:	c3                   	retq   

Disassembly of section .plt:

0000000000400c60 <.plt>:
  400c60:	ff 35 a2 43 20 00    	pushq  0x2043a2(%rip)        # 605008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400c66:	ff 25 a4 43 20 00    	jmpq   *0x2043a4(%rip)        # 605010 <_GLOBAL_OFFSET_TABLE_+0x10>
  400c6c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400c70 <strcasecmp@plt>:
  400c70:	ff 25 a2 43 20 00    	jmpq   *0x2043a2(%rip)        # 605018 <strcasecmp@GLIBC_2.2.5>
  400c76:	68 00 00 00 00       	pushq  $0x0
  400c7b:	e9 e0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400c80 <__errno_location@plt>:
  400c80:	ff 25 9a 43 20 00    	jmpq   *0x20439a(%rip)        # 605020 <__errno_location@GLIBC_2.2.5>
  400c86:	68 01 00 00 00       	pushq  $0x1
  400c8b:	e9 d0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400c90 <srandom@plt>:
  400c90:	ff 25 92 43 20 00    	jmpq   *0x204392(%rip)        # 605028 <srandom@GLIBC_2.2.5>
  400c96:	68 02 00 00 00       	pushq  $0x2
  400c9b:	e9 c0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400ca0 <strncmp@plt>:
  400ca0:	ff 25 8a 43 20 00    	jmpq   *0x20438a(%rip)        # 605030 <strncmp@GLIBC_2.2.5>
  400ca6:	68 03 00 00 00       	pushq  $0x3
  400cab:	e9 b0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cb0 <strcpy@plt>:
  400cb0:	ff 25 82 43 20 00    	jmpq   *0x204382(%rip)        # 605038 <strcpy@GLIBC_2.2.5>
  400cb6:	68 04 00 00 00       	pushq  $0x4
  400cbb:	e9 a0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cc0 <puts@plt>:
  400cc0:	ff 25 7a 43 20 00    	jmpq   *0x20437a(%rip)        # 605040 <puts@GLIBC_2.2.5>
  400cc6:	68 05 00 00 00       	pushq  $0x5
  400ccb:	e9 90 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cd0 <write@plt>:
  400cd0:	ff 25 72 43 20 00    	jmpq   *0x204372(%rip)        # 605048 <write@GLIBC_2.2.5>
  400cd6:	68 06 00 00 00       	pushq  $0x6
  400cdb:	e9 80 ff ff ff       	jmpq   400c60 <.plt>

0000000000400ce0 <__stack_chk_fail@plt>:
  400ce0:	ff 25 6a 43 20 00    	jmpq   *0x20436a(%rip)        # 605050 <__stack_chk_fail@GLIBC_2.4>
  400ce6:	68 07 00 00 00       	pushq  $0x7
  400ceb:	e9 70 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cf0 <mmap@plt>:
  400cf0:	ff 25 62 43 20 00    	jmpq   *0x204362(%rip)        # 605058 <mmap@GLIBC_2.2.5>
  400cf6:	68 08 00 00 00       	pushq  $0x8
  400cfb:	e9 60 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d00 <memset@plt>:
  400d00:	ff 25 5a 43 20 00    	jmpq   *0x20435a(%rip)        # 605060 <memset@GLIBC_2.2.5>
  400d06:	68 09 00 00 00       	pushq  $0x9
  400d0b:	e9 50 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d10 <alarm@plt>:
  400d10:	ff 25 52 43 20 00    	jmpq   *0x204352(%rip)        # 605068 <alarm@GLIBC_2.2.5>
  400d16:	68 0a 00 00 00       	pushq  $0xa
  400d1b:	e9 40 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d20 <close@plt>:
  400d20:	ff 25 4a 43 20 00    	jmpq   *0x20434a(%rip)        # 605070 <close@GLIBC_2.2.5>
  400d26:	68 0b 00 00 00       	pushq  $0xb
  400d2b:	e9 30 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d30 <read@plt>:
  400d30:	ff 25 42 43 20 00    	jmpq   *0x204342(%rip)        # 605078 <read@GLIBC_2.2.5>
  400d36:	68 0c 00 00 00       	pushq  $0xc
  400d3b:	e9 20 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d40 <signal@plt>:
  400d40:	ff 25 3a 43 20 00    	jmpq   *0x20433a(%rip)        # 605080 <signal@GLIBC_2.2.5>
  400d46:	68 0d 00 00 00       	pushq  $0xd
  400d4b:	e9 10 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d50 <gethostbyname@plt>:
  400d50:	ff 25 32 43 20 00    	jmpq   *0x204332(%rip)        # 605088 <gethostbyname@GLIBC_2.2.5>
  400d56:	68 0e 00 00 00       	pushq  $0xe
  400d5b:	e9 00 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d60 <__memmove_chk@plt>:
  400d60:	ff 25 2a 43 20 00    	jmpq   *0x20432a(%rip)        # 605090 <__memmove_chk@GLIBC_2.3.4>
  400d66:	68 0f 00 00 00       	pushq  $0xf
  400d6b:	e9 f0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400d70 <strtol@plt>:
  400d70:	ff 25 22 43 20 00    	jmpq   *0x204322(%rip)        # 605098 <strtol@GLIBC_2.2.5>
  400d76:	68 10 00 00 00       	pushq  $0x10
  400d7b:	e9 e0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400d80 <memcpy@plt>:
  400d80:	ff 25 1a 43 20 00    	jmpq   *0x20431a(%rip)        # 6050a0 <memcpy@GLIBC_2.14>
  400d86:	68 11 00 00 00       	pushq  $0x11
  400d8b:	e9 d0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400d90 <time@plt>:
  400d90:	ff 25 12 43 20 00    	jmpq   *0x204312(%rip)        # 6050a8 <time@GLIBC_2.2.5>
  400d96:	68 12 00 00 00       	pushq  $0x12
  400d9b:	e9 c0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400da0 <random@plt>:
  400da0:	ff 25 0a 43 20 00    	jmpq   *0x20430a(%rip)        # 6050b0 <random@GLIBC_2.2.5>
  400da6:	68 13 00 00 00       	pushq  $0x13
  400dab:	e9 b0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400db0 <_IO_getc@plt>:
  400db0:	ff 25 02 43 20 00    	jmpq   *0x204302(%rip)        # 6050b8 <_IO_getc@GLIBC_2.2.5>
  400db6:	68 14 00 00 00       	pushq  $0x14
  400dbb:	e9 a0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400dc0 <__isoc99_sscanf@plt>:
  400dc0:	ff 25 fa 42 20 00    	jmpq   *0x2042fa(%rip)        # 6050c0 <__isoc99_sscanf@GLIBC_2.7>
  400dc6:	68 15 00 00 00       	pushq  $0x15
  400dcb:	e9 90 fe ff ff       	jmpq   400c60 <.plt>

0000000000400dd0 <munmap@plt>:
  400dd0:	ff 25 f2 42 20 00    	jmpq   *0x2042f2(%rip)        # 6050c8 <munmap@GLIBC_2.2.5>
  400dd6:	68 16 00 00 00       	pushq  $0x16
  400ddb:	e9 80 fe ff ff       	jmpq   400c60 <.plt>

0000000000400de0 <__printf_chk@plt>:
  400de0:	ff 25 ea 42 20 00    	jmpq   *0x2042ea(%rip)        # 6050d0 <__printf_chk@GLIBC_2.3.4>
  400de6:	68 17 00 00 00       	pushq  $0x17
  400deb:	e9 70 fe ff ff       	jmpq   400c60 <.plt>

0000000000400df0 <fopen@plt>:
  400df0:	ff 25 e2 42 20 00    	jmpq   *0x2042e2(%rip)        # 6050d8 <fopen@GLIBC_2.2.5>
  400df6:	68 18 00 00 00       	pushq  $0x18
  400dfb:	e9 60 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e00 <getopt@plt>:
  400e00:	ff 25 da 42 20 00    	jmpq   *0x2042da(%rip)        # 6050e0 <getopt@GLIBC_2.2.5>
  400e06:	68 19 00 00 00       	pushq  $0x19
  400e0b:	e9 50 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e10 <strtoul@plt>:
  400e10:	ff 25 d2 42 20 00    	jmpq   *0x2042d2(%rip)        # 6050e8 <strtoul@GLIBC_2.2.5>
  400e16:	68 1a 00 00 00       	pushq  $0x1a
  400e1b:	e9 40 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e20 <gethostname@plt>:
  400e20:	ff 25 ca 42 20 00    	jmpq   *0x2042ca(%rip)        # 6050f0 <gethostname@GLIBC_2.2.5>
  400e26:	68 1b 00 00 00       	pushq  $0x1b
  400e2b:	e9 30 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e30 <exit@plt>:
  400e30:	ff 25 c2 42 20 00    	jmpq   *0x2042c2(%rip)        # 6050f8 <exit@GLIBC_2.2.5>
  400e36:	68 1c 00 00 00       	pushq  $0x1c
  400e3b:	e9 20 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e40 <connect@plt>:
  400e40:	ff 25 ba 42 20 00    	jmpq   *0x2042ba(%rip)        # 605100 <connect@GLIBC_2.2.5>
  400e46:	68 1d 00 00 00       	pushq  $0x1d
  400e4b:	e9 10 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e50 <__fprintf_chk@plt>:
  400e50:	ff 25 b2 42 20 00    	jmpq   *0x2042b2(%rip)        # 605108 <__fprintf_chk@GLIBC_2.3.4>
  400e56:	68 1e 00 00 00       	pushq  $0x1e
  400e5b:	e9 00 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e60 <__sprintf_chk@plt>:
  400e60:	ff 25 aa 42 20 00    	jmpq   *0x2042aa(%rip)        # 605110 <__sprintf_chk@GLIBC_2.3.4>
  400e66:	68 1f 00 00 00       	pushq  $0x1f
  400e6b:	e9 f0 fd ff ff       	jmpq   400c60 <.plt>

0000000000400e70 <socket@plt>:
  400e70:	ff 25 a2 42 20 00    	jmpq   *0x2042a2(%rip)        # 605118 <socket@GLIBC_2.2.5>
  400e76:	68 20 00 00 00       	pushq  $0x20
  400e7b:	e9 e0 fd ff ff       	jmpq   400c60 <.plt>

Disassembly of section .text:

0000000000400e80 <_start>:
  400e80:	31 ed                	xor    %ebp,%ebp
  400e82:	49 89 d1             	mov    %rdx,%r9
  400e85:	5e                   	pop    %rsi
  400e86:	48 89 e2             	mov    %rsp,%rdx
  400e89:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  400e8d:	50                   	push   %rax
  400e8e:	54                   	push   %rsp
  400e8f:	49 c7 c0 90 2f 40 00 	mov    $0x402f90,%r8
  400e96:	48 c7 c1 20 2f 40 00 	mov    $0x402f20,%rcx
  400e9d:	48 c7 c7 8e 11 40 00 	mov    $0x40118e,%rdi
  400ea4:	ff 15 46 41 20 00    	callq  *0x204146(%rip)        # 604ff0 <__libc_start_main@GLIBC_2.2.5>
  400eaa:	f4                   	hlt    
  400eab:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000400eb0 <_dl_relocate_static_pie>:
  400eb0:	f3 c3                	repz retq 
  400eb2:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400eb9:	00 00 00 
  400ebc:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400ec0 <deregister_tm_clones>:
  400ec0:	55                   	push   %rbp
  400ec1:	b8 90 54 60 00       	mov    $0x605490,%eax
  400ec6:	48 3d 90 54 60 00    	cmp    $0x605490,%rax
  400ecc:	48 89 e5             	mov    %rsp,%rbp
  400ecf:	74 17                	je     400ee8 <deregister_tm_clones+0x28>
  400ed1:	b8 00 00 00 00       	mov    $0x0,%eax
  400ed6:	48 85 c0             	test   %rax,%rax
  400ed9:	74 0d                	je     400ee8 <deregister_tm_clones+0x28>
  400edb:	5d                   	pop    %rbp
  400edc:	bf 90 54 60 00       	mov    $0x605490,%edi
  400ee1:	ff e0                	jmpq   *%rax
  400ee3:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  400ee8:	5d                   	pop    %rbp
  400ee9:	c3                   	retq   
  400eea:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400ef0 <register_tm_clones>:
  400ef0:	be 90 54 60 00       	mov    $0x605490,%esi
  400ef5:	55                   	push   %rbp
  400ef6:	48 81 ee 90 54 60 00 	sub    $0x605490,%rsi
  400efd:	48 89 e5             	mov    %rsp,%rbp
  400f00:	48 c1 fe 03          	sar    $0x3,%rsi
  400f04:	48 89 f0             	mov    %rsi,%rax
  400f07:	48 c1 e8 3f          	shr    $0x3f,%rax
  400f0b:	48 01 c6             	add    %rax,%rsi
  400f0e:	48 d1 fe             	sar    %rsi
  400f11:	74 15                	je     400f28 <register_tm_clones+0x38>
  400f13:	b8 00 00 00 00       	mov    $0x0,%eax
  400f18:	48 85 c0             	test   %rax,%rax
  400f1b:	74 0b                	je     400f28 <register_tm_clones+0x38>
  400f1d:	5d                   	pop    %rbp
  400f1e:	bf 90 54 60 00       	mov    $0x605490,%edi
  400f23:	ff e0                	jmpq   *%rax
  400f25:	0f 1f 00             	nopl   (%rax)
  400f28:	5d                   	pop    %rbp
  400f29:	c3                   	retq   
  400f2a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400f30 <__do_global_dtors_aux>:
  400f30:	80 3d 91 45 20 00 00 	cmpb   $0x0,0x204591(%rip)        # 6054c8 <completed.7698>
  400f37:	75 17                	jne    400f50 <__do_global_dtors_aux+0x20>
  400f39:	55                   	push   %rbp
  400f3a:	48 89 e5             	mov    %rsp,%rbp
  400f3d:	e8 7e ff ff ff       	callq  400ec0 <deregister_tm_clones>
  400f42:	c6 05 7f 45 20 00 01 	movb   $0x1,0x20457f(%rip)        # 6054c8 <completed.7698>
  400f49:	5d                   	pop    %rbp
  400f4a:	c3                   	retq   
  400f4b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  400f50:	f3 c3                	repz retq 
  400f52:	0f 1f 40 00          	nopl   0x0(%rax)
  400f56:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400f5d:	00 00 00 

0000000000400f60 <frame_dummy>:
  400f60:	55                   	push   %rbp
  400f61:	48 89 e5             	mov    %rsp,%rbp
  400f64:	5d                   	pop    %rbp
  400f65:	eb 89                	jmp    400ef0 <register_tm_clones>

0000000000400f67 <usage>:
  400f67:	48 83 ec 08          	sub    $0x8,%rsp
  400f6b:	48 89 fa             	mov    %rdi,%rdx
  400f6e:	83 3d 93 45 20 00 00 	cmpl   $0x0,0x204593(%rip)        # 605508 <is_checker>
  400f75:	74 50                	je     400fc7 <usage+0x60>
  400f77:	48 8d 35 2a 20 00 00 	lea    0x202a(%rip),%rsi        # 402fa8 <_IO_stdin_used+0x8>
  400f7e:	bf 01 00 00 00       	mov    $0x1,%edi
  400f83:	b8 00 00 00 00       	mov    $0x0,%eax
  400f88:	e8 53 fe ff ff       	callq  400de0 <__printf_chk@plt>
  400f8d:	48 8d 3d 4c 20 00 00 	lea    0x204c(%rip),%rdi        # 402fe0 <_IO_stdin_used+0x40>
  400f94:	e8 27 fd ff ff       	callq  400cc0 <puts@plt>
  400f99:	48 8d 3d b8 21 00 00 	lea    0x21b8(%rip),%rdi        # 403158 <_IO_stdin_used+0x1b8>
  400fa0:	e8 1b fd ff ff       	callq  400cc0 <puts@plt>
  400fa5:	48 8d 3d 5c 20 00 00 	lea    0x205c(%rip),%rdi        # 403008 <_IO_stdin_used+0x68>
  400fac:	e8 0f fd ff ff       	callq  400cc0 <puts@plt>
  400fb1:	48 8d 3d ba 21 00 00 	lea    0x21ba(%rip),%rdi        # 403172 <_IO_stdin_used+0x1d2>
  400fb8:	e8 03 fd ff ff       	callq  400cc0 <puts@plt>
  400fbd:	bf 00 00 00 00       	mov    $0x0,%edi
  400fc2:	e8 69 fe ff ff       	callq  400e30 <exit@plt>
  400fc7:	48 8d 35 c0 21 00 00 	lea    0x21c0(%rip),%rsi        # 40318e <_IO_stdin_used+0x1ee>
  400fce:	bf 01 00 00 00       	mov    $0x1,%edi
  400fd3:	b8 00 00 00 00       	mov    $0x0,%eax
  400fd8:	e8 03 fe ff ff       	callq  400de0 <__printf_chk@plt>
  400fdd:	48 8d 3d 4c 20 00 00 	lea    0x204c(%rip),%rdi        # 403030 <_IO_stdin_used+0x90>
  400fe4:	e8 d7 fc ff ff       	callq  400cc0 <puts@plt>
  400fe9:	48 8d 3d 68 20 00 00 	lea    0x2068(%rip),%rdi        # 403058 <_IO_stdin_used+0xb8>
  400ff0:	e8 cb fc ff ff       	callq  400cc0 <puts@plt>
  400ff5:	48 8d 3d b0 21 00 00 	lea    0x21b0(%rip),%rdi        # 4031ac <_IO_stdin_used+0x20c>
  400ffc:	e8 bf fc ff ff       	callq  400cc0 <puts@plt>
  401001:	eb ba                	jmp    400fbd <usage+0x56>

0000000000401003 <initialize_target>:
  401003:	55                   	push   %rbp
  401004:	53                   	push   %rbx
  401005:	48 81 ec 18 21 00 00 	sub    $0x2118,%rsp
  40100c:	89 f5                	mov    %esi,%ebp
  40100e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401015:	00 00 
  401017:	48 89 84 24 08 21 00 	mov    %rax,0x2108(%rsp)
  40101e:	00 
  40101f:	31 c0                	xor    %eax,%eax
  401021:	89 3d d1 44 20 00    	mov    %edi,0x2044d1(%rip)        # 6054f8 <check_level>
  401027:	8b 3d 03 41 20 00    	mov    0x204103(%rip),%edi        # 605130 <target_id>
  40102d:	e8 cc 1e 00 00       	callq  402efe <gencookie>
  401032:	89 05 cc 44 20 00    	mov    %eax,0x2044cc(%rip)        # 605504 <cookie>
  401038:	89 c7                	mov    %eax,%edi
  40103a:	e8 bf 1e 00 00       	callq  402efe <gencookie>
  40103f:	89 05 bb 44 20 00    	mov    %eax,0x2044bb(%rip)        # 605500 <authkey>
  401045:	8b 05 e5 40 20 00    	mov    0x2040e5(%rip),%eax        # 605130 <target_id>
  40104b:	8d 78 01             	lea    0x1(%rax),%edi
  40104e:	e8 3d fc ff ff       	callq  400c90 <srandom@plt>
  401053:	e8 48 fd ff ff       	callq  400da0 <random@plt>
  401058:	89 c7                	mov    %eax,%edi
  40105a:	e8 1a 03 00 00       	callq  401379 <scramble>
  40105f:	89 c3                	mov    %eax,%ebx
  401061:	85 ed                	test   %ebp,%ebp
  401063:	75 54                	jne    4010b9 <initialize_target+0xb6>
  401065:	b8 00 00 00 00       	mov    $0x0,%eax
  40106a:	01 d8                	add    %ebx,%eax
  40106c:	0f b7 c0             	movzwl %ax,%eax
  40106f:	8d 04 c5 00 01 00 00 	lea    0x100(,%rax,8),%eax
  401076:	89 c0                	mov    %eax,%eax
  401078:	48 89 05 01 44 20 00 	mov    %rax,0x204401(%rip)        # 605480 <buf_offset>
  40107f:	c6 05 a2 50 20 00 63 	movb   $0x63,0x2050a2(%rip)        # 606128 <target_prefix>
  401086:	83 3d fb 43 20 00 00 	cmpl   $0x0,0x2043fb(%rip)        # 605488 <notify>
  40108d:	74 09                	je     401098 <initialize_target+0x95>
  40108f:	83 3d 72 44 20 00 00 	cmpl   $0x0,0x204472(%rip)        # 605508 <is_checker>
  401096:	74 39                	je     4010d1 <initialize_target+0xce>
  401098:	48 8b 84 24 08 21 00 	mov    0x2108(%rsp),%rax
  40109f:	00 
  4010a0:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4010a7:	00 00 
  4010a9:	0f 85 da 00 00 00    	jne    401189 <initialize_target+0x186>
  4010af:	48 81 c4 18 21 00 00 	add    $0x2118,%rsp
  4010b6:	5b                   	pop    %rbx
  4010b7:	5d                   	pop    %rbp
  4010b8:	c3                   	retq   
  4010b9:	bf 00 00 00 00       	mov    $0x0,%edi
  4010be:	e8 cd fc ff ff       	callq  400d90 <time@plt>
  4010c3:	89 c7                	mov    %eax,%edi
  4010c5:	e8 c6 fb ff ff       	callq  400c90 <srandom@plt>
  4010ca:	e8 d1 fc ff ff       	callq  400da0 <random@plt>
  4010cf:	eb 99                	jmp    40106a <initialize_target+0x67>
  4010d1:	48 89 e7             	mov    %rsp,%rdi
  4010d4:	be 00 01 00 00       	mov    $0x100,%esi
  4010d9:	e8 42 fd ff ff       	callq  400e20 <gethostname@plt>
  4010de:	89 c5                	mov    %eax,%ebp
  4010e0:	85 c0                	test   %eax,%eax
  4010e2:	75 26                	jne    40110a <initialize_target+0x107>
  4010e4:	89 c3                	mov    %eax,%ebx
  4010e6:	48 63 c3             	movslq %ebx,%rax
  4010e9:	48 8d 15 70 40 20 00 	lea    0x204070(%rip),%rdx        # 605160 <host_table>
  4010f0:	48 8b 3c c2          	mov    (%rdx,%rax,8),%rdi
  4010f4:	48 85 ff             	test   %rdi,%rdi
  4010f7:	74 2c                	je     401125 <initialize_target+0x122>
  4010f9:	48 89 e6             	mov    %rsp,%rsi
  4010fc:	e8 6f fb ff ff       	callq  400c70 <strcasecmp@plt>
  401101:	85 c0                	test   %eax,%eax
  401103:	74 1b                	je     401120 <initialize_target+0x11d>
  401105:	83 c3 01             	add    $0x1,%ebx
  401108:	eb dc                	jmp    4010e6 <initialize_target+0xe3>
  40110a:	48 8d 3d 77 1f 00 00 	lea    0x1f77(%rip),%rdi        # 403088 <_IO_stdin_used+0xe8>
  401111:	e8 aa fb ff ff       	callq  400cc0 <puts@plt>
  401116:	bf 08 00 00 00       	mov    $0x8,%edi
  40111b:	e8 10 fd ff ff       	callq  400e30 <exit@plt>
  401120:	bd 01 00 00 00       	mov    $0x1,%ebp
  401125:	85 ed                	test   %ebp,%ebp
  401127:	74 3d                	je     401166 <initialize_target+0x163>
  401129:	48 8d bc 24 00 01 00 	lea    0x100(%rsp),%rdi
  401130:	00 
  401131:	e8 f8 1a 00 00       	callq  402c2e <init_driver>
  401136:	85 c0                	test   %eax,%eax
  401138:	0f 89 5a ff ff ff    	jns    401098 <initialize_target+0x95>
  40113e:	48 8d 94 24 00 01 00 	lea    0x100(%rsp),%rdx
  401145:	00 
  401146:	48 8d 35 b3 1f 00 00 	lea    0x1fb3(%rip),%rsi        # 403100 <_IO_stdin_used+0x160>
  40114d:	bf 01 00 00 00       	mov    $0x1,%edi
  401152:	b8 00 00 00 00       	mov    $0x0,%eax
  401157:	e8 84 fc ff ff       	callq  400de0 <__printf_chk@plt>
  40115c:	bf 08 00 00 00       	mov    $0x8,%edi
  401161:	e8 ca fc ff ff       	callq  400e30 <exit@plt>
  401166:	48 89 e2             	mov    %rsp,%rdx
  401169:	48 8d 35 50 1f 00 00 	lea    0x1f50(%rip),%rsi        # 4030c0 <_IO_stdin_used+0x120>
  401170:	bf 01 00 00 00       	mov    $0x1,%edi
  401175:	b8 00 00 00 00       	mov    $0x0,%eax
  40117a:	e8 61 fc ff ff       	callq  400de0 <__printf_chk@plt>
  40117f:	bf 08 00 00 00       	mov    $0x8,%edi
  401184:	e8 a7 fc ff ff       	callq  400e30 <exit@plt>
  401189:	e8 52 fb ff ff       	callq  400ce0 <__stack_chk_fail@plt>

000000000040118e <main>:
  40118e:	41 56                	push   %r14
  401190:	41 55                	push   %r13
  401192:	41 54                	push   %r12
  401194:	55                   	push   %rbp
  401195:	53                   	push   %rbx
  401196:	41 89 fc             	mov    %edi,%r12d
  401199:	48 89 f3             	mov    %rsi,%rbx
  40119c:	48 c7 c6 35 1f 40 00 	mov    $0x401f35,%rsi
  4011a3:	bf 0b 00 00 00       	mov    $0xb,%edi
  4011a8:	e8 93 fb ff ff       	callq  400d40 <signal@plt>
  4011ad:	48 c7 c6 e1 1e 40 00 	mov    $0x401ee1,%rsi
  4011b4:	bf 07 00 00 00       	mov    $0x7,%edi
  4011b9:	e8 82 fb ff ff       	callq  400d40 <signal@plt>
  4011be:	48 c7 c6 89 1f 40 00 	mov    $0x401f89,%rsi
  4011c5:	bf 04 00 00 00       	mov    $0x4,%edi
  4011ca:	e8 71 fb ff ff       	callq  400d40 <signal@plt>
  4011cf:	83 3d 32 43 20 00 00 	cmpl   $0x0,0x204332(%rip)        # 605508 <is_checker>
  4011d6:	75 26                	jne    4011fe <main+0x70>
  4011d8:	48 8d 2d e6 1f 00 00 	lea    0x1fe6(%rip),%rbp        # 4031c5 <_IO_stdin_used+0x225>
  4011df:	48 8b 05 ba 42 20 00 	mov    0x2042ba(%rip),%rax        # 6054a0 <stdin@@GLIBC_2.2.5>
  4011e6:	48 89 05 03 43 20 00 	mov    %rax,0x204303(%rip)        # 6054f0 <infile>
  4011ed:	41 bd 00 00 00 00    	mov    $0x0,%r13d
  4011f3:	41 be 00 00 00 00    	mov    $0x0,%r14d
  4011f9:	e9 8d 00 00 00       	jmpq   40128b <main+0xfd>
  4011fe:	48 c7 c6 dd 1f 40 00 	mov    $0x401fdd,%rsi
  401205:	bf 0e 00 00 00       	mov    $0xe,%edi
  40120a:	e8 31 fb ff ff       	callq  400d40 <signal@plt>
  40120f:	bf 05 00 00 00       	mov    $0x5,%edi
  401214:	e8 f7 fa ff ff       	callq  400d10 <alarm@plt>
  401219:	48 8d 2d aa 1f 00 00 	lea    0x1faa(%rip),%rbp        # 4031ca <_IO_stdin_used+0x22a>
  401220:	eb bd                	jmp    4011df <main+0x51>
  401222:	48 8b 3b             	mov    (%rbx),%rdi
  401225:	e8 3d fd ff ff       	callq  400f67 <usage>
  40122a:	48 8d 35 24 22 00 00 	lea    0x2224(%rip),%rsi        # 403455 <_IO_stdin_used+0x4b5>
  401231:	48 8b 3d 70 42 20 00 	mov    0x204270(%rip),%rdi        # 6054a8 <optarg@@GLIBC_2.2.5>
  401238:	e8 b3 fb ff ff       	callq  400df0 <fopen@plt>
  40123d:	48 89 05 ac 42 20 00 	mov    %rax,0x2042ac(%rip)        # 6054f0 <infile>
  401244:	48 85 c0             	test   %rax,%rax
  401247:	75 42                	jne    40128b <main+0xfd>
  401249:	48 8b 0d 58 42 20 00 	mov    0x204258(%rip),%rcx        # 6054a8 <optarg@@GLIBC_2.2.5>
  401250:	48 8d 15 7b 1f 00 00 	lea    0x1f7b(%rip),%rdx        # 4031d2 <_IO_stdin_used+0x232>
  401257:	be 01 00 00 00       	mov    $0x1,%esi
  40125c:	48 8b 3d 5d 42 20 00 	mov    0x20425d(%rip),%rdi        # 6054c0 <stderr@@GLIBC_2.2.5>
  401263:	e8 e8 fb ff ff       	callq  400e50 <__fprintf_chk@plt>
  401268:	b8 01 00 00 00       	mov    $0x1,%eax
  40126d:	e9 d9 00 00 00       	jmpq   40134b <main+0x1bd>
  401272:	ba 10 00 00 00       	mov    $0x10,%edx
  401277:	be 00 00 00 00       	mov    $0x0,%esi
  40127c:	48 8b 3d 25 42 20 00 	mov    0x204225(%rip),%rdi        # 6054a8 <optarg@@GLIBC_2.2.5>
  401283:	e8 88 fb ff ff       	callq  400e10 <strtoul@plt>
  401288:	41 89 c6             	mov    %eax,%r14d
  40128b:	48 89 ea             	mov    %rbp,%rdx
  40128e:	48 89 de             	mov    %rbx,%rsi
  401291:	44 89 e7             	mov    %r12d,%edi
  401294:	e8 67 fb ff ff       	callq  400e00 <getopt@plt>
  401299:	3c ff                	cmp    $0xff,%al
  40129b:	74 62                	je     4012ff <main+0x171>
  40129d:	0f be d0             	movsbl %al,%edx
  4012a0:	83 e8 61             	sub    $0x61,%eax
  4012a3:	3c 10                	cmp    $0x10,%al
  4012a5:	77 3a                	ja     4012e1 <main+0x153>
  4012a7:	0f b6 c0             	movzbl %al,%eax
  4012aa:	48 8d 0d 5f 1f 00 00 	lea    0x1f5f(%rip),%rcx        # 403210 <_IO_stdin_used+0x270>
  4012b1:	48 63 04 81          	movslq (%rcx,%rax,4),%rax
  4012b5:	48 01 c8             	add    %rcx,%rax
  4012b8:	ff e0                	jmpq   *%rax
  4012ba:	ba 0a 00 00 00       	mov    $0xa,%edx
  4012bf:	be 00 00 00 00       	mov    $0x0,%esi
  4012c4:	48 8b 3d dd 41 20 00 	mov    0x2041dd(%rip),%rdi        # 6054a8 <optarg@@GLIBC_2.2.5>
  4012cb:	e8 a0 fa ff ff       	callq  400d70 <strtol@plt>
  4012d0:	41 89 c5             	mov    %eax,%r13d
  4012d3:	eb b6                	jmp    40128b <main+0xfd>
  4012d5:	c7 05 a9 41 20 00 00 	movl   $0x0,0x2041a9(%rip)        # 605488 <notify>
  4012dc:	00 00 00 
  4012df:	eb aa                	jmp    40128b <main+0xfd>
  4012e1:	48 8d 35 07 1f 00 00 	lea    0x1f07(%rip),%rsi        # 4031ef <_IO_stdin_used+0x24f>
  4012e8:	bf 01 00 00 00       	mov    $0x1,%edi
  4012ed:	b8 00 00 00 00       	mov    $0x0,%eax
  4012f2:	e8 e9 fa ff ff       	callq  400de0 <__printf_chk@plt>
  4012f7:	48 8b 3b             	mov    (%rbx),%rdi
  4012fa:	e8 68 fc ff ff       	callq  400f67 <usage>
  4012ff:	be 00 00 00 00       	mov    $0x0,%esi
  401304:	44 89 ef             	mov    %r13d,%edi
  401307:	e8 f7 fc ff ff       	callq  401003 <initialize_target>
  40130c:	83 3d f5 41 20 00 00 	cmpl   $0x0,0x2041f5(%rip)        # 605508 <is_checker>
  401313:	74 09                	je     40131e <main+0x190>
  401315:	44 39 35 e4 41 20 00 	cmp    %r14d,0x2041e4(%rip)        # 605500 <authkey>
  40131c:	75 36                	jne    401354 <main+0x1c6>
  40131e:	8b 15 e0 41 20 00    	mov    0x2041e0(%rip),%edx        # 605504 <cookie>
  401324:	48 8d 35 d7 1e 00 00 	lea    0x1ed7(%rip),%rsi        # 403202 <_IO_stdin_used+0x262>
  40132b:	bf 01 00 00 00       	mov    $0x1,%edi
  401330:	b8 00 00 00 00       	mov    $0x0,%eax
  401335:	e8 a6 fa ff ff       	callq  400de0 <__printf_chk@plt>
  40133a:	48 8b 3d 3f 41 20 00 	mov    0x20413f(%rip),%rdi        # 605480 <buf_offset>
  401341:	e8 a3 0d 00 00       	callq  4020e9 <stable_launch>
  401346:	b8 00 00 00 00       	mov    $0x0,%eax
  40134b:	5b                   	pop    %rbx
  40134c:	5d                   	pop    %rbp
  40134d:	41 5c                	pop    %r12
  40134f:	41 5d                	pop    %r13
  401351:	41 5e                	pop    %r14
  401353:	c3                   	retq   
  401354:	44 89 f2             	mov    %r14d,%edx
  401357:	48 8d 35 ca 1d 00 00 	lea    0x1dca(%rip),%rsi        # 403128 <_IO_stdin_used+0x188>
  40135e:	bf 01 00 00 00       	mov    $0x1,%edi
  401363:	b8 00 00 00 00       	mov    $0x0,%eax
  401368:	e8 73 fa ff ff       	callq  400de0 <__printf_chk@plt>
  40136d:	b8 00 00 00 00       	mov    $0x0,%eax
  401372:	e8 ca 07 00 00       	callq  401b41 <check_fail>
  401377:	eb a5                	jmp    40131e <main+0x190>

0000000000401379 <scramble>:
  401379:	48 83 ec 38          	sub    $0x38,%rsp
  40137d:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401384:	00 00 
  401386:	48 89 44 24 28       	mov    %rax,0x28(%rsp)
  40138b:	31 c0                	xor    %eax,%eax
  40138d:	eb 10                	jmp    40139f <scramble+0x26>
  40138f:	69 d0 fc 6d 00 00    	imul   $0x6dfc,%eax,%edx
  401395:	01 fa                	add    %edi,%edx
  401397:	89 c1                	mov    %eax,%ecx
  401399:	89 14 8c             	mov    %edx,(%rsp,%rcx,4)
  40139c:	83 c0 01             	add    $0x1,%eax
  40139f:	83 f8 09             	cmp    $0x9,%eax
  4013a2:	76 eb                	jbe    40138f <scramble+0x16>
  4013a4:	8b 44 24 18          	mov    0x18(%rsp),%eax
  4013a8:	69 c0 4e e4 00 00    	imul   $0xe44e,%eax,%eax
  4013ae:	89 44 24 18          	mov    %eax,0x18(%rsp)
  4013b2:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4013b6:	69 c0 fe 03 00 00    	imul   $0x3fe,%eax,%eax
  4013bc:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4013c0:	8b 44 24 04          	mov    0x4(%rsp),%eax
  4013c4:	69 c0 17 26 00 00    	imul   $0x2617,%eax,%eax
  4013ca:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4013ce:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4013d2:	69 c0 c0 8e 00 00    	imul   $0x8ec0,%eax,%eax
  4013d8:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4013dc:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4013e0:	69 c0 86 e0 00 00    	imul   $0xe086,%eax,%eax
  4013e6:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4013ea:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4013ee:	69 c0 e9 1f 00 00    	imul   $0x1fe9,%eax,%eax
  4013f4:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4013f8:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  4013fc:	69 c0 3e 71 00 00    	imul   $0x713e,%eax,%eax
  401402:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  401406:	8b 44 24 10          	mov    0x10(%rsp),%eax
  40140a:	69 c0 00 1a 00 00    	imul   $0x1a00,%eax,%eax
  401410:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401414:	8b 44 24 18          	mov    0x18(%rsp),%eax
  401418:	69 c0 b4 0d 00 00    	imul   $0xdb4,%eax,%eax
  40141e:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401422:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401426:	69 c0 1a b6 00 00    	imul   $0xb61a,%eax,%eax
  40142c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401430:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401434:	69 c0 81 51 00 00    	imul   $0x5181,%eax,%eax
  40143a:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40143e:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401442:	69 c0 1c d1 00 00    	imul   $0xd11c,%eax,%eax
  401448:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40144c:	8b 04 24             	mov    (%rsp),%eax
  40144f:	69 c0 a2 8a 00 00    	imul   $0x8aa2,%eax,%eax
  401455:	89 04 24             	mov    %eax,(%rsp)
  401458:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  40145c:	69 c0 ef 0d 00 00    	imul   $0xdef,%eax,%eax
  401462:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  401466:	8b 44 24 18          	mov    0x18(%rsp),%eax
  40146a:	69 c0 72 63 00 00    	imul   $0x6372,%eax,%eax
  401470:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401474:	8b 44 24 18          	mov    0x18(%rsp),%eax
  401478:	69 c0 3f 59 00 00    	imul   $0x593f,%eax,%eax
  40147e:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401482:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401486:	69 c0 b1 b2 00 00    	imul   $0xb2b1,%eax,%eax
  40148c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401490:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401494:	69 c0 31 c7 00 00    	imul   $0xc731,%eax,%eax
  40149a:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40149e:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4014a2:	69 c0 ca c4 00 00    	imul   $0xc4ca,%eax,%eax
  4014a8:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4014ac:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4014b0:	69 c0 47 b4 00 00    	imul   $0xb447,%eax,%eax
  4014b6:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4014ba:	8b 44 24 20          	mov    0x20(%rsp),%eax
  4014be:	69 c0 03 d6 00 00    	imul   $0xd603,%eax,%eax
  4014c4:	89 44 24 20          	mov    %eax,0x20(%rsp)
  4014c8:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4014cc:	69 c0 94 25 00 00    	imul   $0x2594,%eax,%eax
  4014d2:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4014d6:	8b 44 24 18          	mov    0x18(%rsp),%eax
  4014da:	69 c0 2d 87 00 00    	imul   $0x872d,%eax,%eax
  4014e0:	89 44 24 18          	mov    %eax,0x18(%rsp)
  4014e4:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4014e8:	69 c0 e5 fe 00 00    	imul   $0xfee5,%eax,%eax
  4014ee:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4014f2:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4014f6:	69 c0 76 a5 00 00    	imul   $0xa576,%eax,%eax
  4014fc:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401500:	8b 44 24 10          	mov    0x10(%rsp),%eax
  401504:	69 c0 41 55 00 00    	imul   $0x5541,%eax,%eax
  40150a:	89 44 24 10          	mov    %eax,0x10(%rsp)
  40150e:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401512:	69 c0 91 5e 00 00    	imul   $0x5e91,%eax,%eax
  401518:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40151c:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401520:	69 c0 94 3f 00 00    	imul   $0x3f94,%eax,%eax
  401526:	89 44 24 20          	mov    %eax,0x20(%rsp)
  40152a:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40152e:	69 c0 04 3d 00 00    	imul   $0x3d04,%eax,%eax
  401534:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401538:	8b 44 24 18          	mov    0x18(%rsp),%eax
  40153c:	69 c0 77 d4 00 00    	imul   $0xd477,%eax,%eax
  401542:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401546:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40154a:	69 c0 fe bf 00 00    	imul   $0xbffe,%eax,%eax
  401550:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401554:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  401558:	69 c0 36 e8 00 00    	imul   $0xe836,%eax,%eax
  40155e:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  401562:	8b 04 24             	mov    (%rsp),%eax
  401565:	69 c0 f3 8a 00 00    	imul   $0x8af3,%eax,%eax
  40156b:	89 04 24             	mov    %eax,(%rsp)
  40156e:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  401572:	69 c0 77 4b 00 00    	imul   $0x4b77,%eax,%eax
  401578:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  40157c:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401580:	69 c0 5a 05 00 00    	imul   $0x55a,%eax,%eax
  401586:	89 44 24 20          	mov    %eax,0x20(%rsp)
  40158a:	8b 44 24 14          	mov    0x14(%rsp),%eax
  40158e:	69 c0 0e f5 00 00    	imul   $0xf50e,%eax,%eax
  401594:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401598:	8b 44 24 20          	mov    0x20(%rsp),%eax
  40159c:	69 c0 ef d3 00 00    	imul   $0xd3ef,%eax,%eax
  4015a2:	89 44 24 20          	mov    %eax,0x20(%rsp)
  4015a6:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4015aa:	69 c0 9e 22 00 00    	imul   $0x229e,%eax,%eax
  4015b0:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4015b4:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4015b8:	69 c0 37 5a 00 00    	imul   $0x5a37,%eax,%eax
  4015be:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4015c2:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4015c6:	69 c0 fe ef 00 00    	imul   $0xeffe,%eax,%eax
  4015cc:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4015d0:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4015d4:	69 c0 83 d6 00 00    	imul   $0xd683,%eax,%eax
  4015da:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4015de:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4015e2:	69 c0 a5 f6 00 00    	imul   $0xf6a5,%eax,%eax
  4015e8:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4015ec:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4015f0:	69 c0 9d 6b 00 00    	imul   $0x6b9d,%eax,%eax
  4015f6:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4015fa:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4015fe:	69 c0 49 1e 00 00    	imul   $0x1e49,%eax,%eax
  401604:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  401608:	8b 44 24 20          	mov    0x20(%rsp),%eax
  40160c:	69 c0 be 83 00 00    	imul   $0x83be,%eax,%eax
  401612:	89 44 24 20          	mov    %eax,0x20(%rsp)
  401616:	8b 44 24 20          	mov    0x20(%rsp),%eax
  40161a:	69 c0 17 49 00 00    	imul   $0x4917,%eax,%eax
  401620:	89 44 24 20          	mov    %eax,0x20(%rsp)
  401624:	8b 44 24 24          	mov    0x24(%rsp),%eax
  401628:	69 c0 76 20 00 00    	imul   $0x2076,%eax,%eax
  40162e:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401632:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401636:	69 c0 6c b6 00 00    	imul   $0xb66c,%eax,%eax
  40163c:	89 44 24 04          	mov    %eax,0x4(%rsp)
  401640:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401644:	69 c0 b5 e0 00 00    	imul   $0xe0b5,%eax,%eax
  40164a:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40164e:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  401652:	69 c0 26 fb 00 00    	imul   $0xfb26,%eax,%eax
  401658:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  40165c:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401660:	69 c0 a2 01 00 00    	imul   $0x1a2,%eax,%eax
  401666:	89 44 24 14          	mov    %eax,0x14(%rsp)
  40166a:	8b 44 24 24          	mov    0x24(%rsp),%eax
  40166e:	69 c0 6c c1 00 00    	imul   $0xc16c,%eax,%eax
  401674:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401678:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40167c:	69 c0 d7 0a 00 00    	imul   $0xad7,%eax,%eax
  401682:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401686:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40168a:	69 c0 ad 30 00 00    	imul   $0x30ad,%eax,%eax
  401690:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401694:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401698:	69 c0 8e 05 00 00    	imul   $0x58e,%eax,%eax
  40169e:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4016a2:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4016a6:	69 c0 bc 64 00 00    	imul   $0x64bc,%eax,%eax
  4016ac:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4016b0:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4016b4:	69 c0 eb 65 00 00    	imul   $0x65eb,%eax,%eax
  4016ba:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4016be:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4016c2:	69 c0 26 11 00 00    	imul   $0x1126,%eax,%eax
  4016c8:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4016cc:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4016d0:	69 c0 9b 91 00 00    	imul   $0x919b,%eax,%eax
  4016d6:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4016da:	8b 44 24 04          	mov    0x4(%rsp),%eax
  4016de:	69 c0 29 7a 00 00    	imul   $0x7a29,%eax,%eax
  4016e4:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4016e8:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4016ec:	69 c0 2f 22 00 00    	imul   $0x222f,%eax,%eax
  4016f2:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4016f6:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4016fa:	69 c0 16 59 00 00    	imul   $0x5916,%eax,%eax
  401700:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401704:	8b 44 24 10          	mov    0x10(%rsp),%eax
  401708:	69 c0 8e f4 00 00    	imul   $0xf48e,%eax,%eax
  40170e:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401712:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401716:	69 c0 0e ee 00 00    	imul   $0xee0e,%eax,%eax
  40171c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401720:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  401724:	69 c0 65 11 00 00    	imul   $0x1165,%eax,%eax
  40172a:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  40172e:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  401732:	69 c0 53 bb 00 00    	imul   $0xbb53,%eax,%eax
  401738:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  40173c:	8b 44 24 24          	mov    0x24(%rsp),%eax
  401740:	69 c0 6a d9 00 00    	imul   $0xd96a,%eax,%eax
  401746:	89 44 24 24          	mov    %eax,0x24(%rsp)
  40174a:	8b 44 24 24          	mov    0x24(%rsp),%eax
  40174e:	69 c0 98 e8 00 00    	imul   $0xe898,%eax,%eax
  401754:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401758:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40175c:	69 c0 ca 60 00 00    	imul   $0x60ca,%eax,%eax
  401762:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401766:	8b 44 24 10          	mov    0x10(%rsp),%eax
  40176a:	69 c0 50 49 00 00    	imul   $0x4950,%eax,%eax
  401770:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401774:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401778:	69 c0 f2 39 00 00    	imul   $0x39f2,%eax,%eax
  40177e:	89 44 24 20          	mov    %eax,0x20(%rsp)
  401782:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401786:	69 c0 69 42 00 00    	imul   $0x4269,%eax,%eax
  40178c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401790:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401794:	69 c0 6d 09 00 00    	imul   $0x96d,%eax,%eax
  40179a:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40179e:	8b 44 24 04          	mov    0x4(%rsp),%eax
  4017a2:	69 c0 66 71 00 00    	imul   $0x7166,%eax,%eax
  4017a8:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4017ac:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4017b0:	69 c0 ff 82 00 00    	imul   $0x82ff,%eax,%eax
  4017b6:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4017ba:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4017be:	69 c0 62 b0 00 00    	imul   $0xb062,%eax,%eax
  4017c4:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4017c8:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4017cc:	69 c0 d0 ae 00 00    	imul   $0xaed0,%eax,%eax
  4017d2:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4017d6:	8b 04 24             	mov    (%rsp),%eax
  4017d9:	69 c0 d4 e7 00 00    	imul   $0xe7d4,%eax,%eax
  4017df:	89 04 24             	mov    %eax,(%rsp)
  4017e2:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4017e6:	69 c0 fa 18 00 00    	imul   $0x18fa,%eax,%eax
  4017ec:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4017f0:	8b 44 24 18          	mov    0x18(%rsp),%eax
  4017f4:	69 c0 a1 78 00 00    	imul   $0x78a1,%eax,%eax
  4017fa:	89 44 24 18          	mov    %eax,0x18(%rsp)
  4017fe:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401802:	69 c0 18 bf 00 00    	imul   $0xbf18,%eax,%eax
  401808:	89 44 24 20          	mov    %eax,0x20(%rsp)
  40180c:	8b 44 24 10          	mov    0x10(%rsp),%eax
  401810:	69 c0 67 3d 00 00    	imul   $0x3d67,%eax,%eax
  401816:	89 44 24 10          	mov    %eax,0x10(%rsp)
  40181a:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40181e:	69 c0 b4 b0 00 00    	imul   $0xb0b4,%eax,%eax
  401824:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401828:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40182c:	69 c0 c3 7c 00 00    	imul   $0x7cc3,%eax,%eax
  401832:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401836:	8b 44 24 24          	mov    0x24(%rsp),%eax
  40183a:	69 c0 46 3f 00 00    	imul   $0x3f46,%eax,%eax
  401840:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401844:	8b 04 24             	mov    (%rsp),%eax
  401847:	69 c0 d6 77 00 00    	imul   $0x77d6,%eax,%eax
  40184d:	89 04 24             	mov    %eax,(%rsp)
  401850:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401854:	69 c0 1f 74 00 00    	imul   $0x741f,%eax,%eax
  40185a:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40185e:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401862:	69 c0 15 19 00 00    	imul   $0x1915,%eax,%eax
  401868:	89 44 24 14          	mov    %eax,0x14(%rsp)
  40186c:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401870:	69 c0 cf 0d 00 00    	imul   $0xdcf,%eax,%eax
  401876:	89 44 24 14          	mov    %eax,0x14(%rsp)
  40187a:	8b 44 24 14          	mov    0x14(%rsp),%eax
  40187e:	69 c0 96 18 00 00    	imul   $0x1896,%eax,%eax
  401884:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401888:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40188c:	69 c0 0b 76 00 00    	imul   $0x760b,%eax,%eax
  401892:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401896:	8b 44 24 04          	mov    0x4(%rsp),%eax
  40189a:	69 c0 fe 35 00 00    	imul   $0x35fe,%eax,%eax
  4018a0:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4018a4:	ba 00 00 00 00       	mov    $0x0,%edx
  4018a9:	b8 00 00 00 00       	mov    $0x0,%eax
  4018ae:	eb 0a                	jmp    4018ba <scramble+0x541>
  4018b0:	89 d1                	mov    %edx,%ecx
  4018b2:	8b 0c 8c             	mov    (%rsp,%rcx,4),%ecx
  4018b5:	01 c8                	add    %ecx,%eax
  4018b7:	83 c2 01             	add    $0x1,%edx
  4018ba:	83 fa 09             	cmp    $0x9,%edx
  4018bd:	76 f1                	jbe    4018b0 <scramble+0x537>
  4018bf:	48 8b 74 24 28       	mov    0x28(%rsp),%rsi
  4018c4:	64 48 33 34 25 28 00 	xor    %fs:0x28,%rsi
  4018cb:	00 00 
  4018cd:	75 05                	jne    4018d4 <scramble+0x55b>
  4018cf:	48 83 c4 38          	add    $0x38,%rsp
  4018d3:	c3                   	retq   
  4018d4:	e8 07 f4 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

00000000004018d9 <getbuf>:
  4018d9:	48 83 ec 28          	sub    $0x28,%rsp
  4018dd:	48 89 e7             	mov    %rsp,%rdi
  4018e0:	e8 94 02 00 00       	callq  401b79 <Gets>
  4018e5:	b8 01 00 00 00       	mov    $0x1,%eax
  4018ea:	48 83 c4 28          	add    $0x28,%rsp
  4018ee:	c3                   	retq   

00000000004018ef <touch1>:
  4018ef:	48 83 ec 08          	sub    $0x8,%rsp
  4018f3:	c7 05 ff 3b 20 00 01 	movl   $0x1,0x203bff(%rip)        # 6054fc <vlevel>
  4018fa:	00 00 00 
  4018fd:	48 8d 3d a3 19 00 00 	lea    0x19a3(%rip),%rdi        # 4032a7 <_IO_stdin_used+0x307>
  401904:	e8 b7 f3 ff ff       	callq  400cc0 <puts@plt>
  401909:	bf 01 00 00 00       	mov    $0x1,%edi
  40190e:	e8 d6 04 00 00       	callq  401de9 <validate>
  401913:	bf 00 00 00 00       	mov    $0x0,%edi
  401918:	e8 13 f5 ff ff       	callq  400e30 <exit@plt>

000000000040191d <touch2>:
  40191d:	48 83 ec 08          	sub    $0x8,%rsp
  401921:	89 fa                	mov    %edi,%edx
  401923:	c7 05 cf 3b 20 00 02 	movl   $0x2,0x203bcf(%rip)        # 6054fc <vlevel>
  40192a:	00 00 00 
  40192d:	39 3d d1 3b 20 00    	cmp    %edi,0x203bd1(%rip)        # 605504 <cookie>
  401933:	74 2a                	je     40195f <touch2+0x42>
  401935:	48 8d 35 bc 19 00 00 	lea    0x19bc(%rip),%rsi        # 4032f8 <_IO_stdin_used+0x358>
  40193c:	bf 01 00 00 00       	mov    $0x1,%edi
  401941:	b8 00 00 00 00       	mov    $0x0,%eax
  401946:	e8 95 f4 ff ff       	callq  400de0 <__printf_chk@plt>
  40194b:	bf 02 00 00 00       	mov    $0x2,%edi
  401950:	e8 64 05 00 00       	callq  401eb9 <fail>
  401955:	bf 00 00 00 00       	mov    $0x0,%edi
  40195a:	e8 d1 f4 ff ff       	callq  400e30 <exit@plt>
  40195f:	48 8d 35 6a 19 00 00 	lea    0x196a(%rip),%rsi        # 4032d0 <_IO_stdin_used+0x330>
  401966:	bf 01 00 00 00       	mov    $0x1,%edi
  40196b:	b8 00 00 00 00       	mov    $0x0,%eax
  401970:	e8 6b f4 ff ff       	callq  400de0 <__printf_chk@plt>
  401975:	bf 02 00 00 00       	mov    $0x2,%edi
  40197a:	e8 6a 04 00 00       	callq  401de9 <validate>
  40197f:	eb d4                	jmp    401955 <touch2+0x38>

0000000000401981 <hexmatch>:
  401981:	41 54                	push   %r12
  401983:	55                   	push   %rbp
  401984:	53                   	push   %rbx
  401985:	48 83 c4 80          	add    $0xffffffffffffff80,%rsp
  401989:	89 fd                	mov    %edi,%ebp
  40198b:	48 89 f3             	mov    %rsi,%rbx
  40198e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401995:	00 00 
  401997:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
  40199c:	31 c0                	xor    %eax,%eax
  40199e:	e8 fd f3 ff ff       	callq  400da0 <random@plt>
  4019a3:	48 89 c1             	mov    %rax,%rcx
  4019a6:	48 ba 0b d7 a3 70 3d 	movabs $0xa3d70a3d70a3d70b,%rdx
  4019ad:	0a d7 a3 
  4019b0:	48 f7 ea             	imul   %rdx
  4019b3:	48 01 ca             	add    %rcx,%rdx
  4019b6:	48 c1 fa 06          	sar    $0x6,%rdx
  4019ba:	48 89 c8             	mov    %rcx,%rax
  4019bd:	48 c1 f8 3f          	sar    $0x3f,%rax
  4019c1:	48 29 c2             	sub    %rax,%rdx
  4019c4:	48 8d 04 92          	lea    (%rdx,%rdx,4),%rax
  4019c8:	48 8d 14 80          	lea    (%rax,%rax,4),%rdx
  4019cc:	48 8d 04 95 00 00 00 	lea    0x0(,%rdx,4),%rax
  4019d3:	00 
  4019d4:	48 29 c1             	sub    %rax,%rcx
  4019d7:	4c 8d 24 0c          	lea    (%rsp,%rcx,1),%r12
  4019db:	41 89 e8             	mov    %ebp,%r8d
  4019de:	48 8d 0d df 18 00 00 	lea    0x18df(%rip),%rcx        # 4032c4 <_IO_stdin_used+0x324>
  4019e5:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
  4019ec:	be 01 00 00 00       	mov    $0x1,%esi
  4019f1:	4c 89 e7             	mov    %r12,%rdi
  4019f4:	b8 00 00 00 00       	mov    $0x0,%eax
  4019f9:	e8 62 f4 ff ff       	callq  400e60 <__sprintf_chk@plt>
  4019fe:	ba 09 00 00 00       	mov    $0x9,%edx
  401a03:	4c 89 e6             	mov    %r12,%rsi
  401a06:	48 89 df             	mov    %rbx,%rdi
  401a09:	e8 92 f2 ff ff       	callq  400ca0 <strncmp@plt>
  401a0e:	85 c0                	test   %eax,%eax
  401a10:	0f 94 c0             	sete   %al
  401a13:	48 8b 5c 24 78       	mov    0x78(%rsp),%rbx
  401a18:	64 48 33 1c 25 28 00 	xor    %fs:0x28,%rbx
  401a1f:	00 00 
  401a21:	75 0c                	jne    401a2f <hexmatch+0xae>
  401a23:	0f b6 c0             	movzbl %al,%eax
  401a26:	48 83 ec 80          	sub    $0xffffffffffffff80,%rsp
  401a2a:	5b                   	pop    %rbx
  401a2b:	5d                   	pop    %rbp
  401a2c:	41 5c                	pop    %r12
  401a2e:	c3                   	retq   
  401a2f:	e8 ac f2 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000401a34 <touch3>:
  401a34:	53                   	push   %rbx
  401a35:	48 89 fb             	mov    %rdi,%rbx
  401a38:	c7 05 ba 3a 20 00 03 	movl   $0x3,0x203aba(%rip)        # 6054fc <vlevel>
  401a3f:	00 00 00 
  401a42:	48 89 fe             	mov    %rdi,%rsi
  401a45:	8b 3d b9 3a 20 00    	mov    0x203ab9(%rip),%edi        # 605504 <cookie>
  401a4b:	e8 31 ff ff ff       	callq  401981 <hexmatch>
  401a50:	85 c0                	test   %eax,%eax
  401a52:	74 2d                	je     401a81 <touch3+0x4d>
  401a54:	48 89 da             	mov    %rbx,%rdx
  401a57:	48 8d 35 c2 18 00 00 	lea    0x18c2(%rip),%rsi        # 403320 <_IO_stdin_used+0x380>
  401a5e:	bf 01 00 00 00       	mov    $0x1,%edi
  401a63:	b8 00 00 00 00       	mov    $0x0,%eax
  401a68:	e8 73 f3 ff ff       	callq  400de0 <__printf_chk@plt>
  401a6d:	bf 03 00 00 00       	mov    $0x3,%edi
  401a72:	e8 72 03 00 00       	callq  401de9 <validate>
  401a77:	bf 00 00 00 00       	mov    $0x0,%edi
  401a7c:	e8 af f3 ff ff       	callq  400e30 <exit@plt>
  401a81:	48 89 da             	mov    %rbx,%rdx
  401a84:	48 8d 35 bd 18 00 00 	lea    0x18bd(%rip),%rsi        # 403348 <_IO_stdin_used+0x3a8>
  401a8b:	bf 01 00 00 00       	mov    $0x1,%edi
  401a90:	b8 00 00 00 00       	mov    $0x0,%eax
  401a95:	e8 46 f3 ff ff       	callq  400de0 <__printf_chk@plt>
  401a9a:	bf 03 00 00 00       	mov    $0x3,%edi
  401a9f:	e8 15 04 00 00       	callq  401eb9 <fail>
  401aa4:	eb d1                	jmp    401a77 <touch3+0x43>

0000000000401aa6 <test>:
  401aa6:	48 83 ec 08          	sub    $0x8,%rsp
  401aaa:	b8 00 00 00 00       	mov    $0x0,%eax
  401aaf:	e8 25 fe ff ff       	callq  4018d9 <getbuf>
  401ab4:	89 c2                	mov    %eax,%edx
  401ab6:	48 8d 35 b3 18 00 00 	lea    0x18b3(%rip),%rsi        # 403370 <_IO_stdin_used+0x3d0>
  401abd:	bf 01 00 00 00       	mov    $0x1,%edi
  401ac2:	b8 00 00 00 00       	mov    $0x0,%eax
  401ac7:	e8 14 f3 ff ff       	callq  400de0 <__printf_chk@plt>
  401acc:	48 83 c4 08          	add    $0x8,%rsp
  401ad0:	c3                   	retq   

0000000000401ad1 <save_char>:
  401ad1:	8b 05 4d 46 20 00    	mov    0x20464d(%rip),%eax        # 606124 <gets_cnt>
  401ad7:	3d ff 03 00 00       	cmp    $0x3ff,%eax
  401adc:	7f 4a                	jg     401b28 <save_char+0x57>
  401ade:	89 f9                	mov    %edi,%ecx
  401ae0:	c0 e9 04             	shr    $0x4,%cl
  401ae3:	8d 14 40             	lea    (%rax,%rax,2),%edx
  401ae6:	4c 8d 05 a3 1b 00 00 	lea    0x1ba3(%rip),%r8        # 403690 <trans_char>
  401aed:	83 e1 0f             	and    $0xf,%ecx
  401af0:	45 0f b6 0c 08       	movzbl (%r8,%rcx,1),%r9d
  401af5:	48 8d 0d 24 3a 20 00 	lea    0x203a24(%rip),%rcx        # 605520 <gets_buf>
  401afc:	48 63 f2             	movslq %edx,%rsi
  401aff:	44 88 0c 31          	mov    %r9b,(%rcx,%rsi,1)
  401b03:	8d 72 01             	lea    0x1(%rdx),%esi
  401b06:	83 e7 0f             	and    $0xf,%edi
  401b09:	41 0f b6 3c 38       	movzbl (%r8,%rdi,1),%edi
  401b0e:	48 63 f6             	movslq %esi,%rsi
  401b11:	40 88 3c 31          	mov    %dil,(%rcx,%rsi,1)
  401b15:	83 c2 02             	add    $0x2,%edx
  401b18:	48 63 d2             	movslq %edx,%rdx
  401b1b:	c6 04 11 20          	movb   $0x20,(%rcx,%rdx,1)
  401b1f:	83 c0 01             	add    $0x1,%eax
  401b22:	89 05 fc 45 20 00    	mov    %eax,0x2045fc(%rip)        # 606124 <gets_cnt>
  401b28:	f3 c3                	repz retq 

0000000000401b2a <save_term>:
  401b2a:	8b 05 f4 45 20 00    	mov    0x2045f4(%rip),%eax        # 606124 <gets_cnt>
  401b30:	8d 04 40             	lea    (%rax,%rax,2),%eax
  401b33:	48 98                	cltq   
  401b35:	48 8d 15 e4 39 20 00 	lea    0x2039e4(%rip),%rdx        # 605520 <gets_buf>
  401b3c:	c6 04 02 00          	movb   $0x0,(%rdx,%rax,1)
  401b40:	c3                   	retq   

0000000000401b41 <check_fail>:
  401b41:	48 83 ec 08          	sub    $0x8,%rsp
  401b45:	0f be 15 dc 45 20 00 	movsbl 0x2045dc(%rip),%edx        # 606128 <target_prefix>
  401b4c:	4c 8d 05 cd 39 20 00 	lea    0x2039cd(%rip),%r8        # 605520 <gets_buf>
  401b53:	8b 0d 9f 39 20 00    	mov    0x20399f(%rip),%ecx        # 6054f8 <check_level>
  401b59:	48 8d 35 33 18 00 00 	lea    0x1833(%rip),%rsi        # 403393 <_IO_stdin_used+0x3f3>
  401b60:	bf 01 00 00 00       	mov    $0x1,%edi
  401b65:	b8 00 00 00 00       	mov    $0x0,%eax
  401b6a:	e8 71 f2 ff ff       	callq  400de0 <__printf_chk@plt>
  401b6f:	bf 01 00 00 00       	mov    $0x1,%edi
  401b74:	e8 b7 f2 ff ff       	callq  400e30 <exit@plt>

0000000000401b79 <Gets>:
  401b79:	41 54                	push   %r12
  401b7b:	55                   	push   %rbp
  401b7c:	53                   	push   %rbx
  401b7d:	49 89 fc             	mov    %rdi,%r12
  401b80:	c7 05 9a 45 20 00 00 	movl   $0x0,0x20459a(%rip)        # 606124 <gets_cnt>
  401b87:	00 00 00 
  401b8a:	48 89 fb             	mov    %rdi,%rbx
  401b8d:	eb 11                	jmp    401ba0 <Gets+0x27>
  401b8f:	48 8d 6b 01          	lea    0x1(%rbx),%rbp
  401b93:	88 03                	mov    %al,(%rbx)
  401b95:	0f b6 f8             	movzbl %al,%edi
  401b98:	e8 34 ff ff ff       	callq  401ad1 <save_char>
  401b9d:	48 89 eb             	mov    %rbp,%rbx
  401ba0:	48 8b 3d 49 39 20 00 	mov    0x203949(%rip),%rdi        # 6054f0 <infile>
  401ba7:	e8 04 f2 ff ff       	callq  400db0 <_IO_getc@plt>
  401bac:	83 f8 ff             	cmp    $0xffffffff,%eax
  401baf:	74 05                	je     401bb6 <Gets+0x3d>
  401bb1:	83 f8 0a             	cmp    $0xa,%eax
  401bb4:	75 d9                	jne    401b8f <Gets+0x16>
  401bb6:	c6 03 00             	movb   $0x0,(%rbx)
  401bb9:	b8 00 00 00 00       	mov    $0x0,%eax
  401bbe:	e8 67 ff ff ff       	callq  401b2a <save_term>
  401bc3:	4c 89 e0             	mov    %r12,%rax
  401bc6:	5b                   	pop    %rbx
  401bc7:	5d                   	pop    %rbp
  401bc8:	41 5c                	pop    %r12
  401bca:	c3                   	retq   

0000000000401bcb <notify_server>:
  401bcb:	55                   	push   %rbp
  401bcc:	53                   	push   %rbx
  401bcd:	48 81 ec 18 40 00 00 	sub    $0x4018,%rsp
  401bd4:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401bdb:	00 00 
  401bdd:	48 89 84 24 08 40 00 	mov    %rax,0x4008(%rsp)
  401be4:	00 
  401be5:	31 c0                	xor    %eax,%eax
  401be7:	83 3d 1a 39 20 00 00 	cmpl   $0x0,0x20391a(%rip)        # 605508 <is_checker>
  401bee:	0f 85 d2 00 00 00    	jne    401cc6 <notify_server+0xfb>
  401bf4:	89 fb                	mov    %edi,%ebx
  401bf6:	8b 05 28 45 20 00    	mov    0x204528(%rip),%eax        # 606124 <gets_cnt>
  401bfc:	83 c0 64             	add    $0x64,%eax
  401bff:	3d 00 20 00 00       	cmp    $0x2000,%eax
  401c04:	0f 8f dd 00 00 00    	jg     401ce7 <notify_server+0x11c>
  401c0a:	0f be 05 17 45 20 00 	movsbl 0x204517(%rip),%eax        # 606128 <target_prefix>
  401c11:	83 3d 70 38 20 00 00 	cmpl   $0x0,0x203870(%rip)        # 605488 <notify>
  401c18:	0f 84 e9 00 00 00    	je     401d07 <notify_server+0x13c>
  401c1e:	8b 15 dc 38 20 00    	mov    0x2038dc(%rip),%edx        # 605500 <authkey>
  401c24:	85 db                	test   %ebx,%ebx
  401c26:	0f 84 e5 00 00 00    	je     401d11 <notify_server+0x146>
  401c2c:	48 8d 2d 76 17 00 00 	lea    0x1776(%rip),%rbp        # 4033a9 <_IO_stdin_used+0x409>
  401c33:	48 89 e7             	mov    %rsp,%rdi
  401c36:	48 8d 0d e3 38 20 00 	lea    0x2038e3(%rip),%rcx        # 605520 <gets_buf>
  401c3d:	51                   	push   %rcx
  401c3e:	56                   	push   %rsi
  401c3f:	50                   	push   %rax
  401c40:	52                   	push   %rdx
  401c41:	49 89 e9             	mov    %rbp,%r9
  401c44:	44 8b 05 e5 34 20 00 	mov    0x2034e5(%rip),%r8d        # 605130 <target_id>
  401c4b:	48 8d 0d 61 17 00 00 	lea    0x1761(%rip),%rcx        # 4033b3 <_IO_stdin_used+0x413>
  401c52:	ba 00 20 00 00       	mov    $0x2000,%edx
  401c57:	be 01 00 00 00       	mov    $0x1,%esi
  401c5c:	b8 00 00 00 00       	mov    $0x0,%eax
  401c61:	e8 fa f1 ff ff       	callq  400e60 <__sprintf_chk@plt>
  401c66:	48 83 c4 20          	add    $0x20,%rsp
  401c6a:	83 3d 17 38 20 00 00 	cmpl   $0x0,0x203817(%rip)        # 605488 <notify>
  401c71:	0f 84 df 00 00 00    	je     401d56 <notify_server+0x18b>
  401c77:	85 db                	test   %ebx,%ebx
  401c79:	0f 84 c6 00 00 00    	je     401d45 <notify_server+0x17a>
  401c7f:	48 89 e1             	mov    %rsp,%rcx
  401c82:	4c 8d 8c 24 00 20 00 	lea    0x2000(%rsp),%r9
  401c89:	00 
  401c8a:	41 b8 00 00 00 00    	mov    $0x0,%r8d
  401c90:	48 8b 15 b1 34 20 00 	mov    0x2034b1(%rip),%rdx        # 605148 <lab>
  401c97:	48 8b 35 b2 34 20 00 	mov    0x2034b2(%rip),%rsi        # 605150 <course>
  401c9e:	48 8b 3d 9b 34 20 00 	mov    0x20349b(%rip),%rdi        # 605140 <user_id>
  401ca5:	e8 af 11 00 00       	callq  402e59 <driver_post>
  401caa:	85 c0                	test   %eax,%eax
  401cac:	78 6f                	js     401d1d <notify_server+0x152>
  401cae:	48 8d 3d 43 18 00 00 	lea    0x1843(%rip),%rdi        # 4034f8 <_IO_stdin_used+0x558>
  401cb5:	e8 06 f0 ff ff       	callq  400cc0 <puts@plt>
  401cba:	48 8d 3d 1a 17 00 00 	lea    0x171a(%rip),%rdi        # 4033db <_IO_stdin_used+0x43b>
  401cc1:	e8 fa ef ff ff       	callq  400cc0 <puts@plt>
  401cc6:	48 8b 84 24 08 40 00 	mov    0x4008(%rsp),%rax
  401ccd:	00 
  401cce:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  401cd5:	00 00 
  401cd7:	0f 85 07 01 00 00    	jne    401de4 <notify_server+0x219>
  401cdd:	48 81 c4 18 40 00 00 	add    $0x4018,%rsp
  401ce4:	5b                   	pop    %rbx
  401ce5:	5d                   	pop    %rbp
  401ce6:	c3                   	retq   
  401ce7:	48 8d 35 da 17 00 00 	lea    0x17da(%rip),%rsi        # 4034c8 <_IO_stdin_used+0x528>
  401cee:	bf 01 00 00 00       	mov    $0x1,%edi
  401cf3:	b8 00 00 00 00       	mov    $0x0,%eax
  401cf8:	e8 e3 f0 ff ff       	callq  400de0 <__printf_chk@plt>
  401cfd:	bf 01 00 00 00       	mov    $0x1,%edi
  401d02:	e8 29 f1 ff ff       	callq  400e30 <exit@plt>
  401d07:	ba ff ff ff ff       	mov    $0xffffffff,%edx
  401d0c:	e9 13 ff ff ff       	jmpq   401c24 <notify_server+0x59>
  401d11:	48 8d 2d 96 16 00 00 	lea    0x1696(%rip),%rbp        # 4033ae <_IO_stdin_used+0x40e>
  401d18:	e9 16 ff ff ff       	jmpq   401c33 <notify_server+0x68>
  401d1d:	48 8d 94 24 00 20 00 	lea    0x2000(%rsp),%rdx
  401d24:	00 
  401d25:	48 8d 35 a3 16 00 00 	lea    0x16a3(%rip),%rsi        # 4033cf <_IO_stdin_used+0x42f>
  401d2c:	bf 01 00 00 00       	mov    $0x1,%edi
  401d31:	b8 00 00 00 00       	mov    $0x0,%eax
  401d36:	e8 a5 f0 ff ff       	callq  400de0 <__printf_chk@plt>
  401d3b:	bf 01 00 00 00       	mov    $0x1,%edi
  401d40:	e8 eb f0 ff ff       	callq  400e30 <exit@plt>
  401d45:	48 8d 3d 99 16 00 00 	lea    0x1699(%rip),%rdi        # 4033e5 <_IO_stdin_used+0x445>
  401d4c:	e8 6f ef ff ff       	callq  400cc0 <puts@plt>
  401d51:	e9 70 ff ff ff       	jmpq   401cc6 <notify_server+0xfb>
  401d56:	48 89 ea             	mov    %rbp,%rdx
  401d59:	48 8d 35 d0 17 00 00 	lea    0x17d0(%rip),%rsi        # 403530 <_IO_stdin_used+0x590>
  401d60:	bf 01 00 00 00       	mov    $0x1,%edi
  401d65:	b8 00 00 00 00       	mov    $0x0,%eax
  401d6a:	e8 71 f0 ff ff       	callq  400de0 <__printf_chk@plt>
  401d6f:	48 8b 15 ca 33 20 00 	mov    0x2033ca(%rip),%rdx        # 605140 <user_id>
  401d76:	48 8d 35 6f 16 00 00 	lea    0x166f(%rip),%rsi        # 4033ec <_IO_stdin_used+0x44c>
  401d7d:	bf 01 00 00 00       	mov    $0x1,%edi
  401d82:	b8 00 00 00 00       	mov    $0x0,%eax
  401d87:	e8 54 f0 ff ff       	callq  400de0 <__printf_chk@plt>
  401d8c:	48 8b 15 bd 33 20 00 	mov    0x2033bd(%rip),%rdx        # 605150 <course>
  401d93:	48 8d 35 5f 16 00 00 	lea    0x165f(%rip),%rsi        # 4033f9 <_IO_stdin_used+0x459>
  401d9a:	bf 01 00 00 00       	mov    $0x1,%edi
  401d9f:	b8 00 00 00 00       	mov    $0x0,%eax
  401da4:	e8 37 f0 ff ff       	callq  400de0 <__printf_chk@plt>
  401da9:	48 8b 15 98 33 20 00 	mov    0x203398(%rip),%rdx        # 605148 <lab>
  401db0:	48 8d 35 4e 16 00 00 	lea    0x164e(%rip),%rsi        # 403405 <_IO_stdin_used+0x465>
  401db7:	bf 01 00 00 00       	mov    $0x1,%edi
  401dbc:	b8 00 00 00 00       	mov    $0x0,%eax
  401dc1:	e8 1a f0 ff ff       	callq  400de0 <__printf_chk@plt>
  401dc6:	48 89 e2             	mov    %rsp,%rdx
  401dc9:	48 8d 35 3e 16 00 00 	lea    0x163e(%rip),%rsi        # 40340e <_IO_stdin_used+0x46e>
  401dd0:	bf 01 00 00 00       	mov    $0x1,%edi
  401dd5:	b8 00 00 00 00       	mov    $0x0,%eax
  401dda:	e8 01 f0 ff ff       	callq  400de0 <__printf_chk@plt>
  401ddf:	e9 e2 fe ff ff       	jmpq   401cc6 <notify_server+0xfb>
  401de4:	e8 f7 ee ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000401de9 <validate>:
  401de9:	53                   	push   %rbx
  401dea:	89 fb                	mov    %edi,%ebx
  401dec:	83 3d 15 37 20 00 00 	cmpl   $0x0,0x203715(%rip)        # 605508 <is_checker>
  401df3:	74 72                	je     401e67 <validate+0x7e>
  401df5:	39 3d 01 37 20 00    	cmp    %edi,0x203701(%rip)        # 6054fc <vlevel>
  401dfb:	75 32                	jne    401e2f <validate+0x46>
  401dfd:	8b 15 f5 36 20 00    	mov    0x2036f5(%rip),%edx        # 6054f8 <check_level>
  401e03:	39 fa                	cmp    %edi,%edx
  401e05:	75 3e                	jne    401e45 <validate+0x5c>
  401e07:	0f be 15 1a 43 20 00 	movsbl 0x20431a(%rip),%edx        # 606128 <target_prefix>
  401e0e:	4c 8d 05 0b 37 20 00 	lea    0x20370b(%rip),%r8        # 605520 <gets_buf>
  401e15:	89 f9                	mov    %edi,%ecx
  401e17:	48 8d 35 1a 16 00 00 	lea    0x161a(%rip),%rsi        # 403438 <_IO_stdin_used+0x498>
  401e1e:	bf 01 00 00 00       	mov    $0x1,%edi
  401e23:	b8 00 00 00 00       	mov    $0x0,%eax
  401e28:	e8 b3 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401e2d:	5b                   	pop    %rbx
  401e2e:	c3                   	retq   
  401e2f:	48 8d 3d e4 15 00 00 	lea    0x15e4(%rip),%rdi        # 40341a <_IO_stdin_used+0x47a>
  401e36:	e8 85 ee ff ff       	callq  400cc0 <puts@plt>
  401e3b:	b8 00 00 00 00       	mov    $0x0,%eax
  401e40:	e8 fc fc ff ff       	callq  401b41 <check_fail>
  401e45:	89 f9                	mov    %edi,%ecx
  401e47:	48 8d 35 0a 17 00 00 	lea    0x170a(%rip),%rsi        # 403558 <_IO_stdin_used+0x5b8>
  401e4e:	bf 01 00 00 00       	mov    $0x1,%edi
  401e53:	b8 00 00 00 00       	mov    $0x0,%eax
  401e58:	e8 83 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401e5d:	b8 00 00 00 00       	mov    $0x0,%eax
  401e62:	e8 da fc ff ff       	callq  401b41 <check_fail>
  401e67:	39 3d 8f 36 20 00    	cmp    %edi,0x20368f(%rip)        # 6054fc <vlevel>
  401e6d:	74 1a                	je     401e89 <validate+0xa0>
  401e6f:	48 8d 3d a4 15 00 00 	lea    0x15a4(%rip),%rdi        # 40341a <_IO_stdin_used+0x47a>
  401e76:	e8 45 ee ff ff       	callq  400cc0 <puts@plt>
  401e7b:	89 de                	mov    %ebx,%esi
  401e7d:	bf 00 00 00 00       	mov    $0x0,%edi
  401e82:	e8 44 fd ff ff       	callq  401bcb <notify_server>
  401e87:	eb a4                	jmp    401e2d <validate+0x44>
  401e89:	0f be 0d 98 42 20 00 	movsbl 0x204298(%rip),%ecx        # 606128 <target_prefix>
  401e90:	89 fa                	mov    %edi,%edx
  401e92:	48 8d 35 e7 16 00 00 	lea    0x16e7(%rip),%rsi        # 403580 <_IO_stdin_used+0x5e0>
  401e99:	bf 01 00 00 00       	mov    $0x1,%edi
  401e9e:	b8 00 00 00 00       	mov    $0x0,%eax
  401ea3:	e8 38 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401ea8:	89 de                	mov    %ebx,%esi
  401eaa:	bf 01 00 00 00       	mov    $0x1,%edi
  401eaf:	e8 17 fd ff ff       	callq  401bcb <notify_server>
  401eb4:	e9 74 ff ff ff       	jmpq   401e2d <validate+0x44>

0000000000401eb9 <fail>:
  401eb9:	48 83 ec 08          	sub    $0x8,%rsp
  401ebd:	83 3d 44 36 20 00 00 	cmpl   $0x0,0x203644(%rip)        # 605508 <is_checker>
  401ec4:	75 11                	jne    401ed7 <fail+0x1e>
  401ec6:	89 fe                	mov    %edi,%esi
  401ec8:	bf 00 00 00 00       	mov    $0x0,%edi
  401ecd:	e8 f9 fc ff ff       	callq  401bcb <notify_server>
  401ed2:	48 83 c4 08          	add    $0x8,%rsp
  401ed6:	c3                   	retq   
  401ed7:	b8 00 00 00 00       	mov    $0x0,%eax
  401edc:	e8 60 fc ff ff       	callq  401b41 <check_fail>

0000000000401ee1 <bushandler>:
  401ee1:	48 83 ec 08          	sub    $0x8,%rsp
  401ee5:	83 3d 1c 36 20 00 00 	cmpl   $0x0,0x20361c(%rip)        # 605508 <is_checker>
  401eec:	74 16                	je     401f04 <bushandler+0x23>
  401eee:	48 8d 3d 58 15 00 00 	lea    0x1558(%rip),%rdi        # 40344d <_IO_stdin_used+0x4ad>
  401ef5:	e8 c6 ed ff ff       	callq  400cc0 <puts@plt>
  401efa:	b8 00 00 00 00       	mov    $0x0,%eax
  401eff:	e8 3d fc ff ff       	callq  401b41 <check_fail>
  401f04:	48 8d 3d ad 16 00 00 	lea    0x16ad(%rip),%rdi        # 4035b8 <_IO_stdin_used+0x618>
  401f0b:	e8 b0 ed ff ff       	callq  400cc0 <puts@plt>
  401f10:	48 8d 3d 40 15 00 00 	lea    0x1540(%rip),%rdi        # 403457 <_IO_stdin_used+0x4b7>
  401f17:	e8 a4 ed ff ff       	callq  400cc0 <puts@plt>
  401f1c:	be 00 00 00 00       	mov    $0x0,%esi
  401f21:	bf 00 00 00 00       	mov    $0x0,%edi
  401f26:	e8 a0 fc ff ff       	callq  401bcb <notify_server>
  401f2b:	bf 01 00 00 00       	mov    $0x1,%edi
  401f30:	e8 fb ee ff ff       	callq  400e30 <exit@plt>

0000000000401f35 <seghandler>:
  401f35:	48 83 ec 08          	sub    $0x8,%rsp
  401f39:	83 3d c8 35 20 00 00 	cmpl   $0x0,0x2035c8(%rip)        # 605508 <is_checker>
  401f40:	74 16                	je     401f58 <seghandler+0x23>
  401f42:	48 8d 3d 24 15 00 00 	lea    0x1524(%rip),%rdi        # 40346d <_IO_stdin_used+0x4cd>
  401f49:	e8 72 ed ff ff       	callq  400cc0 <puts@plt>
  401f4e:	b8 00 00 00 00       	mov    $0x0,%eax
  401f53:	e8 e9 fb ff ff       	callq  401b41 <check_fail>
  401f58:	48 8d 3d 79 16 00 00 	lea    0x1679(%rip),%rdi        # 4035d8 <_IO_stdin_used+0x638>
  401f5f:	e8 5c ed ff ff       	callq  400cc0 <puts@plt>
  401f64:	48 8d 3d ec 14 00 00 	lea    0x14ec(%rip),%rdi        # 403457 <_IO_stdin_used+0x4b7>
  401f6b:	e8 50 ed ff ff       	callq  400cc0 <puts@plt>
  401f70:	be 00 00 00 00       	mov    $0x0,%esi
  401f75:	bf 00 00 00 00       	mov    $0x0,%edi
  401f7a:	e8 4c fc ff ff       	callq  401bcb <notify_server>
  401f7f:	bf 01 00 00 00       	mov    $0x1,%edi
  401f84:	e8 a7 ee ff ff       	callq  400e30 <exit@plt>

0000000000401f89 <illegalhandler>:
  401f89:	48 83 ec 08          	sub    $0x8,%rsp
  401f8d:	83 3d 74 35 20 00 00 	cmpl   $0x0,0x203574(%rip)        # 605508 <is_checker>
  401f94:	74 16                	je     401fac <illegalhandler+0x23>
  401f96:	48 8d 3d e3 14 00 00 	lea    0x14e3(%rip),%rdi        # 403480 <_IO_stdin_used+0x4e0>
  401f9d:	e8 1e ed ff ff       	callq  400cc0 <puts@plt>
  401fa2:	b8 00 00 00 00       	mov    $0x0,%eax
  401fa7:	e8 95 fb ff ff       	callq  401b41 <check_fail>
  401fac:	48 8d 3d 4d 16 00 00 	lea    0x164d(%rip),%rdi        # 403600 <_IO_stdin_used+0x660>
  401fb3:	e8 08 ed ff ff       	callq  400cc0 <puts@plt>
  401fb8:	48 8d 3d 98 14 00 00 	lea    0x1498(%rip),%rdi        # 403457 <_IO_stdin_used+0x4b7>
  401fbf:	e8 fc ec ff ff       	callq  400cc0 <puts@plt>
  401fc4:	be 00 00 00 00       	mov    $0x0,%esi
  401fc9:	bf 00 00 00 00       	mov    $0x0,%edi
  401fce:	e8 f8 fb ff ff       	callq  401bcb <notify_server>
  401fd3:	bf 01 00 00 00       	mov    $0x1,%edi
  401fd8:	e8 53 ee ff ff       	callq  400e30 <exit@plt>

0000000000401fdd <sigalrmhandler>:
  401fdd:	48 83 ec 08          	sub    $0x8,%rsp
  401fe1:	83 3d 20 35 20 00 00 	cmpl   $0x0,0x203520(%rip)        # 605508 <is_checker>
  401fe8:	74 16                	je     402000 <sigalrmhandler+0x23>
  401fea:	48 8d 3d a3 14 00 00 	lea    0x14a3(%rip),%rdi        # 403494 <_IO_stdin_used+0x4f4>
  401ff1:	e8 ca ec ff ff       	callq  400cc0 <puts@plt>
  401ff6:	b8 00 00 00 00       	mov    $0x0,%eax
  401ffb:	e8 41 fb ff ff       	callq  401b41 <check_fail>
  402000:	ba 05 00 00 00       	mov    $0x5,%edx
  402005:	48 8d 35 24 16 00 00 	lea    0x1624(%rip),%rsi        # 403630 <_IO_stdin_used+0x690>
  40200c:	bf 01 00 00 00       	mov    $0x1,%edi
  402011:	b8 00 00 00 00       	mov    $0x0,%eax
  402016:	e8 c5 ed ff ff       	callq  400de0 <__printf_chk@plt>
  40201b:	be 00 00 00 00       	mov    $0x0,%esi
  402020:	bf 00 00 00 00       	mov    $0x0,%edi
  402025:	e8 a1 fb ff ff       	callq  401bcb <notify_server>
  40202a:	bf 01 00 00 00       	mov    $0x1,%edi
  40202f:	e8 fc ed ff ff       	callq  400e30 <exit@plt>

0000000000402034 <launch>:
  402034:	55                   	push   %rbp
  402035:	48 89 e5             	mov    %rsp,%rbp
  402038:	48 83 ec 10          	sub    $0x10,%rsp
  40203c:	48 89 fa             	mov    %rdi,%rdx
  40203f:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402046:	00 00 
  402048:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  40204c:	31 c0                	xor    %eax,%eax
  40204e:	48 8d 47 1e          	lea    0x1e(%rdi),%rax
  402052:	48 83 e0 f0          	and    $0xfffffffffffffff0,%rax
  402056:	48 29 c4             	sub    %rax,%rsp
  402059:	48 8d 7c 24 0f       	lea    0xf(%rsp),%rdi
  40205e:	48 83 e7 f0          	and    $0xfffffffffffffff0,%rdi
  402062:	be f4 00 00 00       	mov    $0xf4,%esi
  402067:	e8 94 ec ff ff       	callq  400d00 <memset@plt>
  40206c:	48 8b 05 2d 34 20 00 	mov    0x20342d(%rip),%rax        # 6054a0 <stdin@@GLIBC_2.2.5>
  402073:	48 39 05 76 34 20 00 	cmp    %rax,0x203476(%rip)        # 6054f0 <infile>
  40207a:	74 3a                	je     4020b6 <launch+0x82>
  40207c:	c7 05 76 34 20 00 00 	movl   $0x0,0x203476(%rip)        # 6054fc <vlevel>
  402083:	00 00 00 
  402086:	b8 00 00 00 00       	mov    $0x0,%eax
  40208b:	e8 16 fa ff ff       	callq  401aa6 <test>
  402090:	83 3d 71 34 20 00 00 	cmpl   $0x0,0x203471(%rip)        # 605508 <is_checker>
  402097:	75 35                	jne    4020ce <launch+0x9a>
  402099:	48 8d 3d 14 14 00 00 	lea    0x1414(%rip),%rdi        # 4034b4 <_IO_stdin_used+0x514>
  4020a0:	e8 1b ec ff ff       	callq  400cc0 <puts@plt>
  4020a5:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4020a9:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4020b0:	00 00 
  4020b2:	75 30                	jne    4020e4 <launch+0xb0>
  4020b4:	c9                   	leaveq 
  4020b5:	c3                   	retq   
  4020b6:	48 8d 35 df 13 00 00 	lea    0x13df(%rip),%rsi        # 40349c <_IO_stdin_used+0x4fc>
  4020bd:	bf 01 00 00 00       	mov    $0x1,%edi
  4020c2:	b8 00 00 00 00       	mov    $0x0,%eax
  4020c7:	e8 14 ed ff ff       	callq  400de0 <__printf_chk@plt>
  4020cc:	eb ae                	jmp    40207c <launch+0x48>
  4020ce:	48 8d 3d d4 13 00 00 	lea    0x13d4(%rip),%rdi        # 4034a9 <_IO_stdin_used+0x509>
  4020d5:	e8 e6 eb ff ff       	callq  400cc0 <puts@plt>
  4020da:	b8 00 00 00 00       	mov    $0x0,%eax
  4020df:	e8 5d fa ff ff       	callq  401b41 <check_fail>
  4020e4:	e8 f7 eb ff ff       	callq  400ce0 <__stack_chk_fail@plt>

00000000004020e9 <stable_launch>:
  4020e9:	53                   	push   %rbx
  4020ea:	48 89 3d f7 33 20 00 	mov    %rdi,0x2033f7(%rip)        # 6054e8 <global_offset>
  4020f1:	41 b9 00 00 00 00    	mov    $0x0,%r9d
  4020f7:	41 b8 00 00 00 00    	mov    $0x0,%r8d
  4020fd:	b9 32 01 00 00       	mov    $0x132,%ecx
  402102:	ba 07 00 00 00       	mov    $0x7,%edx
  402107:	be 00 00 10 00       	mov    $0x100000,%esi
  40210c:	bf 00 60 58 55       	mov    $0x55586000,%edi
  402111:	e8 da eb ff ff       	callq  400cf0 <mmap@plt>
  402116:	48 89 c3             	mov    %rax,%rbx
  402119:	48 3d 00 60 58 55    	cmp    $0x55586000,%rax
  40211f:	75 43                	jne    402164 <stable_launch+0x7b>
  402121:	48 8d 90 f8 ff 0f 00 	lea    0xffff8(%rax),%rdx
  402128:	48 89 15 01 40 20 00 	mov    %rdx,0x204001(%rip)        # 606130 <stack_top>
  40212f:	48 89 e0             	mov    %rsp,%rax
  402132:	48 89 d4             	mov    %rdx,%rsp
  402135:	48 89 c2             	mov    %rax,%rdx
  402138:	48 89 15 a1 33 20 00 	mov    %rdx,0x2033a1(%rip)        # 6054e0 <global_save_stack>
  40213f:	48 8b 3d a2 33 20 00 	mov    0x2033a2(%rip),%rdi        # 6054e8 <global_offset>
  402146:	e8 e9 fe ff ff       	callq  402034 <launch>
  40214b:	48 8b 05 8e 33 20 00 	mov    0x20338e(%rip),%rax        # 6054e0 <global_save_stack>
  402152:	48 89 c4             	mov    %rax,%rsp
  402155:	be 00 00 10 00       	mov    $0x100000,%esi
  40215a:	48 89 df             	mov    %rbx,%rdi
  40215d:	e8 6e ec ff ff       	callq  400dd0 <munmap@plt>
  402162:	5b                   	pop    %rbx
  402163:	c3                   	retq   
  402164:	be 00 00 10 00       	mov    $0x100000,%esi
  402169:	48 89 c7             	mov    %rax,%rdi
  40216c:	e8 5f ec ff ff       	callq  400dd0 <munmap@plt>
  402171:	b9 00 60 58 55       	mov    $0x55586000,%ecx
  402176:	48 8d 15 eb 14 00 00 	lea    0x14eb(%rip),%rdx        # 403668 <_IO_stdin_used+0x6c8>
  40217d:	be 01 00 00 00       	mov    $0x1,%esi
  402182:	48 8b 3d 37 33 20 00 	mov    0x203337(%rip),%rdi        # 6054c0 <stderr@@GLIBC_2.2.5>
  402189:	b8 00 00 00 00       	mov    $0x0,%eax
  40218e:	e8 bd ec ff ff       	callq  400e50 <__fprintf_chk@plt>
  402193:	bf 01 00 00 00       	mov    $0x1,%edi
  402198:	e8 93 ec ff ff       	callq  400e30 <exit@plt>

000000000040219d <rio_readinitb>:
  40219d:	89 37                	mov    %esi,(%rdi)
  40219f:	c7 47 04 00 00 00 00 	movl   $0x0,0x4(%rdi)
  4021a6:	48 8d 47 10          	lea    0x10(%rdi),%rax
  4021aa:	48 89 47 08          	mov    %rax,0x8(%rdi)
  4021ae:	c3                   	retq   

00000000004021af <sigalrm_handler>:
  4021af:	48 83 ec 08          	sub    $0x8,%rsp
  4021b3:	b9 00 00 00 00       	mov    $0x0,%ecx
  4021b8:	48 8d 15 e1 14 00 00 	lea    0x14e1(%rip),%rdx        # 4036a0 <trans_char+0x10>
  4021bf:	be 01 00 00 00       	mov    $0x1,%esi
  4021c4:	48 8b 3d f5 32 20 00 	mov    0x2032f5(%rip),%rdi        # 6054c0 <stderr@@GLIBC_2.2.5>
  4021cb:	b8 00 00 00 00       	mov    $0x0,%eax
  4021d0:	e8 7b ec ff ff       	callq  400e50 <__fprintf_chk@plt>
  4021d5:	bf 01 00 00 00       	mov    $0x1,%edi
  4021da:	e8 51 ec ff ff       	callq  400e30 <exit@plt>

00000000004021df <rio_writen>:
  4021df:	41 55                	push   %r13
  4021e1:	41 54                	push   %r12
  4021e3:	55                   	push   %rbp
  4021e4:	53                   	push   %rbx
  4021e5:	48 83 ec 08          	sub    $0x8,%rsp
  4021e9:	41 89 fc             	mov    %edi,%r12d
  4021ec:	48 89 f5             	mov    %rsi,%rbp
  4021ef:	49 89 d5             	mov    %rdx,%r13
  4021f2:	48 89 d3             	mov    %rdx,%rbx
  4021f5:	eb 06                	jmp    4021fd <rio_writen+0x1e>
  4021f7:	48 29 c3             	sub    %rax,%rbx
  4021fa:	48 01 c5             	add    %rax,%rbp
  4021fd:	48 85 db             	test   %rbx,%rbx
  402200:	74 24                	je     402226 <rio_writen+0x47>
  402202:	48 89 da             	mov    %rbx,%rdx
  402205:	48 89 ee             	mov    %rbp,%rsi
  402208:	44 89 e7             	mov    %r12d,%edi
  40220b:	e8 c0 ea ff ff       	callq  400cd0 <write@plt>
  402210:	48 85 c0             	test   %rax,%rax
  402213:	7f e2                	jg     4021f7 <rio_writen+0x18>
  402215:	e8 66 ea ff ff       	callq  400c80 <__errno_location@plt>
  40221a:	83 38 04             	cmpl   $0x4,(%rax)
  40221d:	75 15                	jne    402234 <rio_writen+0x55>
  40221f:	b8 00 00 00 00       	mov    $0x0,%eax
  402224:	eb d1                	jmp    4021f7 <rio_writen+0x18>
  402226:	4c 89 e8             	mov    %r13,%rax
  402229:	48 83 c4 08          	add    $0x8,%rsp
  40222d:	5b                   	pop    %rbx
  40222e:	5d                   	pop    %rbp
  40222f:	41 5c                	pop    %r12
  402231:	41 5d                	pop    %r13
  402233:	c3                   	retq   
  402234:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
  40223b:	eb ec                	jmp    402229 <rio_writen+0x4a>

000000000040223d <rio_read>:
  40223d:	41 55                	push   %r13
  40223f:	41 54                	push   %r12
  402241:	55                   	push   %rbp
  402242:	53                   	push   %rbx
  402243:	48 83 ec 08          	sub    $0x8,%rsp
  402247:	48 89 fb             	mov    %rdi,%rbx
  40224a:	49 89 f5             	mov    %rsi,%r13
  40224d:	49 89 d4             	mov    %rdx,%r12
  402250:	eb 0a                	jmp    40225c <rio_read+0x1f>
  402252:	e8 29 ea ff ff       	callq  400c80 <__errno_location@plt>
  402257:	83 38 04             	cmpl   $0x4,(%rax)
  40225a:	75 5c                	jne    4022b8 <rio_read+0x7b>
  40225c:	8b 6b 04             	mov    0x4(%rbx),%ebp
  40225f:	85 ed                	test   %ebp,%ebp
  402261:	7f 24                	jg     402287 <rio_read+0x4a>
  402263:	48 8d 6b 10          	lea    0x10(%rbx),%rbp
  402267:	8b 3b                	mov    (%rbx),%edi
  402269:	ba 00 20 00 00       	mov    $0x2000,%edx
  40226e:	48 89 ee             	mov    %rbp,%rsi
  402271:	e8 ba ea ff ff       	callq  400d30 <read@plt>
  402276:	89 43 04             	mov    %eax,0x4(%rbx)
  402279:	85 c0                	test   %eax,%eax
  40227b:	78 d5                	js     402252 <rio_read+0x15>
  40227d:	85 c0                	test   %eax,%eax
  40227f:	74 40                	je     4022c1 <rio_read+0x84>
  402281:	48 89 6b 08          	mov    %rbp,0x8(%rbx)
  402285:	eb d5                	jmp    40225c <rio_read+0x1f>
  402287:	89 e8                	mov    %ebp,%eax
  402289:	4c 39 e0             	cmp    %r12,%rax
  40228c:	72 03                	jb     402291 <rio_read+0x54>
  40228e:	44 89 e5             	mov    %r12d,%ebp
  402291:	4c 63 e5             	movslq %ebp,%r12
  402294:	48 8b 73 08          	mov    0x8(%rbx),%rsi
  402298:	4c 89 e2             	mov    %r12,%rdx
  40229b:	4c 89 ef             	mov    %r13,%rdi
  40229e:	e8 dd ea ff ff       	callq  400d80 <memcpy@plt>
  4022a3:	4c 01 63 08          	add    %r12,0x8(%rbx)
  4022a7:	29 6b 04             	sub    %ebp,0x4(%rbx)
  4022aa:	4c 89 e0             	mov    %r12,%rax
  4022ad:	48 83 c4 08          	add    $0x8,%rsp
  4022b1:	5b                   	pop    %rbx
  4022b2:	5d                   	pop    %rbp
  4022b3:	41 5c                	pop    %r12
  4022b5:	41 5d                	pop    %r13
  4022b7:	c3                   	retq   
  4022b8:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
  4022bf:	eb ec                	jmp    4022ad <rio_read+0x70>
  4022c1:	b8 00 00 00 00       	mov    $0x0,%eax
  4022c6:	eb e5                	jmp    4022ad <rio_read+0x70>

00000000004022c8 <rio_readlineb>:
  4022c8:	41 55                	push   %r13
  4022ca:	41 54                	push   %r12
  4022cc:	55                   	push   %rbp
  4022cd:	53                   	push   %rbx
  4022ce:	48 83 ec 18          	sub    $0x18,%rsp
  4022d2:	49 89 fd             	mov    %rdi,%r13
  4022d5:	48 89 f5             	mov    %rsi,%rbp
  4022d8:	49 89 d4             	mov    %rdx,%r12
  4022db:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4022e2:	00 00 
  4022e4:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  4022e9:	31 c0                	xor    %eax,%eax
  4022eb:	bb 01 00 00 00       	mov    $0x1,%ebx
  4022f0:	4c 39 e3             	cmp    %r12,%rbx
  4022f3:	73 47                	jae    40233c <rio_readlineb+0x74>
  4022f5:	48 8d 74 24 07       	lea    0x7(%rsp),%rsi
  4022fa:	ba 01 00 00 00       	mov    $0x1,%edx
  4022ff:	4c 89 ef             	mov    %r13,%rdi
  402302:	e8 36 ff ff ff       	callq  40223d <rio_read>
  402307:	83 f8 01             	cmp    $0x1,%eax
  40230a:	75 1c                	jne    402328 <rio_readlineb+0x60>
  40230c:	48 8d 45 01          	lea    0x1(%rbp),%rax
  402310:	0f b6 54 24 07       	movzbl 0x7(%rsp),%edx
  402315:	88 55 00             	mov    %dl,0x0(%rbp)
  402318:	80 7c 24 07 0a       	cmpb   $0xa,0x7(%rsp)
  40231d:	74 1a                	je     402339 <rio_readlineb+0x71>
  40231f:	48 83 c3 01          	add    $0x1,%rbx
  402323:	48 89 c5             	mov    %rax,%rbp
  402326:	eb c8                	jmp    4022f0 <rio_readlineb+0x28>
  402328:	85 c0                	test   %eax,%eax
  40232a:	75 32                	jne    40235e <rio_readlineb+0x96>
  40232c:	48 83 fb 01          	cmp    $0x1,%rbx
  402330:	75 0a                	jne    40233c <rio_readlineb+0x74>
  402332:	b8 00 00 00 00       	mov    $0x0,%eax
  402337:	eb 0a                	jmp    402343 <rio_readlineb+0x7b>
  402339:	48 89 c5             	mov    %rax,%rbp
  40233c:	c6 45 00 00          	movb   $0x0,0x0(%rbp)
  402340:	48 89 d8             	mov    %rbx,%rax
  402343:	48 8b 4c 24 08       	mov    0x8(%rsp),%rcx
  402348:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  40234f:	00 00 
  402351:	75 14                	jne    402367 <rio_readlineb+0x9f>
  402353:	48 83 c4 18          	add    $0x18,%rsp
  402357:	5b                   	pop    %rbx
  402358:	5d                   	pop    %rbp
  402359:	41 5c                	pop    %r12
  40235b:	41 5d                	pop    %r13
  40235d:	c3                   	retq   
  40235e:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
  402365:	eb dc                	jmp    402343 <rio_readlineb+0x7b>
  402367:	e8 74 e9 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

000000000040236c <urlencode>:
  40236c:	41 54                	push   %r12
  40236e:	55                   	push   %rbp
  40236f:	53                   	push   %rbx
  402370:	48 83 ec 10          	sub    $0x10,%rsp
  402374:	48 89 fb             	mov    %rdi,%rbx
  402377:	48 89 f5             	mov    %rsi,%rbp
  40237a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402381:	00 00 
  402383:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  402388:	31 c0                	xor    %eax,%eax
  40238a:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  402391:	f2 ae                	repnz scas %es:(%rdi),%al
  402393:	48 89 ce             	mov    %rcx,%rsi
  402396:	48 f7 d6             	not    %rsi
  402399:	8d 46 ff             	lea    -0x1(%rsi),%eax
  40239c:	eb 0f                	jmp    4023ad <urlencode+0x41>
  40239e:	44 88 45 00          	mov    %r8b,0x0(%rbp)
  4023a2:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
  4023a6:	48 83 c3 01          	add    $0x1,%rbx
  4023aa:	44 89 e0             	mov    %r12d,%eax
  4023ad:	44 8d 60 ff          	lea    -0x1(%rax),%r12d
  4023b1:	85 c0                	test   %eax,%eax
  4023b3:	0f 84 a8 00 00 00    	je     402461 <urlencode+0xf5>
  4023b9:	44 0f b6 03          	movzbl (%rbx),%r8d
  4023bd:	41 80 f8 2a          	cmp    $0x2a,%r8b
  4023c1:	0f 94 c2             	sete   %dl
  4023c4:	41 80 f8 2d          	cmp    $0x2d,%r8b
  4023c8:	0f 94 c0             	sete   %al
  4023cb:	08 c2                	or     %al,%dl
  4023cd:	75 cf                	jne    40239e <urlencode+0x32>
  4023cf:	41 80 f8 2e          	cmp    $0x2e,%r8b
  4023d3:	74 c9                	je     40239e <urlencode+0x32>
  4023d5:	41 80 f8 5f          	cmp    $0x5f,%r8b
  4023d9:	74 c3                	je     40239e <urlencode+0x32>
  4023db:	41 8d 40 d0          	lea    -0x30(%r8),%eax
  4023df:	3c 09                	cmp    $0x9,%al
  4023e1:	76 bb                	jbe    40239e <urlencode+0x32>
  4023e3:	41 8d 40 bf          	lea    -0x41(%r8),%eax
  4023e7:	3c 19                	cmp    $0x19,%al
  4023e9:	76 b3                	jbe    40239e <urlencode+0x32>
  4023eb:	41 8d 40 9f          	lea    -0x61(%r8),%eax
  4023ef:	3c 19                	cmp    $0x19,%al
  4023f1:	76 ab                	jbe    40239e <urlencode+0x32>
  4023f3:	41 80 f8 20          	cmp    $0x20,%r8b
  4023f7:	74 56                	je     40244f <urlencode+0xe3>
  4023f9:	41 8d 40 e0          	lea    -0x20(%r8),%eax
  4023fd:	3c 5f                	cmp    $0x5f,%al
  4023ff:	0f 96 c2             	setbe  %dl
  402402:	41 80 f8 09          	cmp    $0x9,%r8b
  402406:	0f 94 c0             	sete   %al
  402409:	08 c2                	or     %al,%dl
  40240b:	74 4f                	je     40245c <urlencode+0xf0>
  40240d:	48 89 e7             	mov    %rsp,%rdi
  402410:	45 0f b6 c0          	movzbl %r8b,%r8d
  402414:	48 8d 0d 1d 13 00 00 	lea    0x131d(%rip),%rcx        # 403738 <trans_char+0xa8>
  40241b:	ba 08 00 00 00       	mov    $0x8,%edx
  402420:	be 01 00 00 00       	mov    $0x1,%esi
  402425:	b8 00 00 00 00       	mov    $0x0,%eax
  40242a:	e8 31 ea ff ff       	callq  400e60 <__sprintf_chk@plt>
  40242f:	0f b6 04 24          	movzbl (%rsp),%eax
  402433:	88 45 00             	mov    %al,0x0(%rbp)
  402436:	0f b6 44 24 01       	movzbl 0x1(%rsp),%eax
  40243b:	88 45 01             	mov    %al,0x1(%rbp)
  40243e:	0f b6 44 24 02       	movzbl 0x2(%rsp),%eax
  402443:	88 45 02             	mov    %al,0x2(%rbp)
  402446:	48 8d 6d 03          	lea    0x3(%rbp),%rbp
  40244a:	e9 57 ff ff ff       	jmpq   4023a6 <urlencode+0x3a>
  40244f:	c6 45 00 2b          	movb   $0x2b,0x0(%rbp)
  402453:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
  402457:	e9 4a ff ff ff       	jmpq   4023a6 <urlencode+0x3a>
  40245c:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402461:	48 8b 74 24 08       	mov    0x8(%rsp),%rsi
  402466:	64 48 33 34 25 28 00 	xor    %fs:0x28,%rsi
  40246d:	00 00 
  40246f:	75 09                	jne    40247a <urlencode+0x10e>
  402471:	48 83 c4 10          	add    $0x10,%rsp
  402475:	5b                   	pop    %rbx
  402476:	5d                   	pop    %rbp
  402477:	41 5c                	pop    %r12
  402479:	c3                   	retq   
  40247a:	e8 61 e8 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

000000000040247f <submitr>:
  40247f:	41 57                	push   %r15
  402481:	41 56                	push   %r14
  402483:	41 55                	push   %r13
  402485:	41 54                	push   %r12
  402487:	55                   	push   %rbp
  402488:	53                   	push   %rbx
  402489:	48 81 ec 68 a0 00 00 	sub    $0xa068,%rsp
  402490:	49 89 fd             	mov    %rdi,%r13
  402493:	89 74 24 14          	mov    %esi,0x14(%rsp)
  402497:	49 89 d7             	mov    %rdx,%r15
  40249a:	48 89 4c 24 08       	mov    %rcx,0x8(%rsp)
  40249f:	4c 89 44 24 18       	mov    %r8,0x18(%rsp)
  4024a4:	4d 89 ce             	mov    %r9,%r14
  4024a7:	48 8b ac 24 a0 a0 00 	mov    0xa0a0(%rsp),%rbp
  4024ae:	00 
  4024af:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4024b6:	00 00 
  4024b8:	48 89 84 24 58 a0 00 	mov    %rax,0xa058(%rsp)
  4024bf:	00 
  4024c0:	31 c0                	xor    %eax,%eax
  4024c2:	c7 44 24 2c 00 00 00 	movl   $0x0,0x2c(%rsp)
  4024c9:	00 
  4024ca:	ba 00 00 00 00       	mov    $0x0,%edx
  4024cf:	be 01 00 00 00       	mov    $0x1,%esi
  4024d4:	bf 02 00 00 00       	mov    $0x2,%edi
  4024d9:	e8 92 e9 ff ff       	callq  400e70 <socket@plt>
  4024de:	85 c0                	test   %eax,%eax
  4024e0:	0f 88 a9 02 00 00    	js     40278f <submitr+0x310>
  4024e6:	89 c3                	mov    %eax,%ebx
  4024e8:	4c 89 ef             	mov    %r13,%rdi
  4024eb:	e8 60 e8 ff ff       	callq  400d50 <gethostbyname@plt>
  4024f0:	48 85 c0             	test   %rax,%rax
  4024f3:	0f 84 e2 02 00 00    	je     4027db <submitr+0x35c>
  4024f9:	4c 8d 64 24 30       	lea    0x30(%rsp),%r12
  4024fe:	48 c7 44 24 32 00 00 	movq   $0x0,0x32(%rsp)
  402505:	00 00 
  402507:	c7 44 24 3a 00 00 00 	movl   $0x0,0x3a(%rsp)
  40250e:	00 
  40250f:	66 c7 44 24 3e 00 00 	movw   $0x0,0x3e(%rsp)
  402516:	66 c7 44 24 30 02 00 	movw   $0x2,0x30(%rsp)
  40251d:	48 63 50 14          	movslq 0x14(%rax),%rdx
  402521:	48 8b 40 18          	mov    0x18(%rax),%rax
  402525:	48 8b 30             	mov    (%rax),%rsi
  402528:	48 8d 7c 24 34       	lea    0x34(%rsp),%rdi
  40252d:	b9 0c 00 00 00       	mov    $0xc,%ecx
  402532:	e8 29 e8 ff ff       	callq  400d60 <__memmove_chk@plt>
  402537:	0f b7 44 24 14       	movzwl 0x14(%rsp),%eax
  40253c:	66 c1 c8 08          	ror    $0x8,%ax
  402540:	66 89 44 24 32       	mov    %ax,0x32(%rsp)
  402545:	ba 10 00 00 00       	mov    $0x10,%edx
  40254a:	4c 89 e6             	mov    %r12,%rsi
  40254d:	89 df                	mov    %ebx,%edi
  40254f:	e8 ec e8 ff ff       	callq  400e40 <connect@plt>
  402554:	85 c0                	test   %eax,%eax
  402556:	0f 88 e7 02 00 00    	js     402843 <submitr+0x3c4>
  40255c:	48 c7 c6 ff ff ff ff 	mov    $0xffffffffffffffff,%rsi
  402563:	b8 00 00 00 00       	mov    $0x0,%eax
  402568:	48 89 f1             	mov    %rsi,%rcx
  40256b:	4c 89 f7             	mov    %r14,%rdi
  40256e:	f2 ae                	repnz scas %es:(%rdi),%al
  402570:	48 89 ca             	mov    %rcx,%rdx
  402573:	48 f7 d2             	not    %rdx
  402576:	48 89 f1             	mov    %rsi,%rcx
  402579:	4c 89 ff             	mov    %r15,%rdi
  40257c:	f2 ae                	repnz scas %es:(%rdi),%al
  40257e:	48 f7 d1             	not    %rcx
  402581:	49 89 c8             	mov    %rcx,%r8
  402584:	48 89 f1             	mov    %rsi,%rcx
  402587:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
  40258c:	f2 ae                	repnz scas %es:(%rdi),%al
  40258e:	48 f7 d1             	not    %rcx
  402591:	4d 8d 44 08 fe       	lea    -0x2(%r8,%rcx,1),%r8
  402596:	48 89 f1             	mov    %rsi,%rcx
  402599:	48 8b 7c 24 18       	mov    0x18(%rsp),%rdi
  40259e:	f2 ae                	repnz scas %es:(%rdi),%al
  4025a0:	48 89 c8             	mov    %rcx,%rax
  4025a3:	48 f7 d0             	not    %rax
  4025a6:	49 8d 4c 00 ff       	lea    -0x1(%r8,%rax,1),%rcx
  4025ab:	48 8d 44 52 fd       	lea    -0x3(%rdx,%rdx,2),%rax
  4025b0:	48 8d 84 01 80 00 00 	lea    0x80(%rcx,%rax,1),%rax
  4025b7:	00 
  4025b8:	48 3d 00 20 00 00    	cmp    $0x2000,%rax
  4025be:	0f 87 d9 02 00 00    	ja     40289d <submitr+0x41e>
  4025c4:	48 8d b4 24 50 40 00 	lea    0x4050(%rsp),%rsi
  4025cb:	00 
  4025cc:	b9 00 04 00 00       	mov    $0x400,%ecx
  4025d1:	b8 00 00 00 00       	mov    $0x0,%eax
  4025d6:	48 89 f7             	mov    %rsi,%rdi
  4025d9:	f3 48 ab             	rep stos %rax,%es:(%rdi)
  4025dc:	4c 89 f7             	mov    %r14,%rdi
  4025df:	e8 88 fd ff ff       	callq  40236c <urlencode>
  4025e4:	85 c0                	test   %eax,%eax
  4025e6:	0f 88 24 03 00 00    	js     402910 <submitr+0x491>
  4025ec:	4c 8d a4 24 50 20 00 	lea    0x2050(%rsp),%r12
  4025f3:	00 
  4025f4:	41 55                	push   %r13
  4025f6:	48 8d 84 24 58 40 00 	lea    0x4058(%rsp),%rax
  4025fd:	00 
  4025fe:	50                   	push   %rax
  4025ff:	4d 89 f9             	mov    %r15,%r9
  402602:	4c 8b 44 24 18       	mov    0x18(%rsp),%r8
  402607:	48 8d 0d ba 10 00 00 	lea    0x10ba(%rip),%rcx        # 4036c8 <trans_char+0x38>
  40260e:	ba 00 20 00 00       	mov    $0x2000,%edx
  402613:	be 01 00 00 00       	mov    $0x1,%esi
  402618:	4c 89 e7             	mov    %r12,%rdi
  40261b:	b8 00 00 00 00       	mov    $0x0,%eax
  402620:	e8 3b e8 ff ff       	callq  400e60 <__sprintf_chk@plt>
  402625:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  40262c:	b8 00 00 00 00       	mov    $0x0,%eax
  402631:	4c 89 e7             	mov    %r12,%rdi
  402634:	f2 ae                	repnz scas %es:(%rdi),%al
  402636:	48 89 ca             	mov    %rcx,%rdx
  402639:	48 f7 d2             	not    %rdx
  40263c:	48 8d 52 ff          	lea    -0x1(%rdx),%rdx
  402640:	4c 89 e6             	mov    %r12,%rsi
  402643:	89 df                	mov    %ebx,%edi
  402645:	e8 95 fb ff ff       	callq  4021df <rio_writen>
  40264a:	48 83 c4 10          	add    $0x10,%rsp
  40264e:	48 85 c0             	test   %rax,%rax
  402651:	0f 88 44 03 00 00    	js     40299b <submitr+0x51c>
  402657:	4c 8d 64 24 40       	lea    0x40(%rsp),%r12
  40265c:	89 de                	mov    %ebx,%esi
  40265e:	4c 89 e7             	mov    %r12,%rdi
  402661:	e8 37 fb ff ff       	callq  40219d <rio_readinitb>
  402666:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  40266d:	00 
  40266e:	ba 00 20 00 00       	mov    $0x2000,%edx
  402673:	4c 89 e7             	mov    %r12,%rdi
  402676:	e8 4d fc ff ff       	callq  4022c8 <rio_readlineb>
  40267b:	48 85 c0             	test   %rax,%rax
  40267e:	0f 8e 86 03 00 00    	jle    402a0a <submitr+0x58b>
  402684:	48 8d 4c 24 2c       	lea    0x2c(%rsp),%rcx
  402689:	48 8d 94 24 50 60 00 	lea    0x6050(%rsp),%rdx
  402690:	00 
  402691:	48 8d bc 24 50 20 00 	lea    0x2050(%rsp),%rdi
  402698:	00 
  402699:	4c 8d 84 24 50 80 00 	lea    0x8050(%rsp),%r8
  4026a0:	00 
  4026a1:	48 8d 35 97 10 00 00 	lea    0x1097(%rip),%rsi        # 40373f <trans_char+0xaf>
  4026a8:	b8 00 00 00 00       	mov    $0x0,%eax
  4026ad:	e8 0e e7 ff ff       	callq  400dc0 <__isoc99_sscanf@plt>
  4026b2:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  4026b9:	00 
  4026ba:	b9 03 00 00 00       	mov    $0x3,%ecx
  4026bf:	48 8d 3d 90 10 00 00 	lea    0x1090(%rip),%rdi        # 403756 <trans_char+0xc6>
  4026c6:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  4026c8:	0f 97 c0             	seta   %al
  4026cb:	1c 00                	sbb    $0x0,%al
  4026cd:	84 c0                	test   %al,%al
  4026cf:	0f 84 b3 03 00 00    	je     402a88 <submitr+0x609>
  4026d5:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  4026dc:	00 
  4026dd:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
  4026e2:	ba 00 20 00 00       	mov    $0x2000,%edx
  4026e7:	e8 dc fb ff ff       	callq  4022c8 <rio_readlineb>
  4026ec:	48 85 c0             	test   %rax,%rax
  4026ef:	7f c1                	jg     4026b2 <submitr+0x233>
  4026f1:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  4026f8:	3a 20 43 
  4026fb:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402702:	20 75 6e 
  402705:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402709:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  40270d:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402714:	74 6f 20 
  402717:	48 ba 72 65 61 64 20 	movabs $0x6165682064616572,%rdx
  40271e:	68 65 61 
  402721:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402725:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402729:	48 b8 64 65 72 73 20 	movabs $0x6f72662073726564,%rax
  402730:	66 72 6f 
  402733:	48 ba 6d 20 74 68 65 	movabs $0x657220656874206d,%rdx
  40273a:	20 72 65 
  40273d:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402741:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402745:	48 b8 73 75 6c 74 20 	movabs $0x72657320746c7573,%rax
  40274c:	73 65 72 
  40274f:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402753:	c7 45 38 76 65 72 00 	movl   $0x726576,0x38(%rbp)
  40275a:	89 df                	mov    %ebx,%edi
  40275c:	e8 bf e5 ff ff       	callq  400d20 <close@plt>
  402761:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402766:	48 8b 9c 24 58 a0 00 	mov    0xa058(%rsp),%rbx
  40276d:	00 
  40276e:	64 48 33 1c 25 28 00 	xor    %fs:0x28,%rbx
  402775:	00 00 
  402777:	0f 85 7e 04 00 00    	jne    402bfb <submitr+0x77c>
  40277d:	48 81 c4 68 a0 00 00 	add    $0xa068,%rsp
  402784:	5b                   	pop    %rbx
  402785:	5d                   	pop    %rbp
  402786:	41 5c                	pop    %r12
  402788:	41 5d                	pop    %r13
  40278a:	41 5e                	pop    %r14
  40278c:	41 5f                	pop    %r15
  40278e:	c3                   	retq   
  40278f:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402796:	3a 20 43 
  402799:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  4027a0:	20 75 6e 
  4027a3:	48 89 45 00          	mov    %rax,0x0(%rbp)
  4027a7:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  4027ab:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  4027b2:	74 6f 20 
  4027b5:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
  4027bc:	65 20 73 
  4027bf:	48 89 45 10          	mov    %rax,0x10(%rbp)
  4027c3:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  4027c7:	c7 45 20 6f 63 6b 65 	movl   $0x656b636f,0x20(%rbp)
  4027ce:	66 c7 45 24 74 00    	movw   $0x74,0x24(%rbp)
  4027d4:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  4027d9:	eb 8b                	jmp    402766 <submitr+0x2e7>
  4027db:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
  4027e2:	3a 20 44 
  4027e5:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
  4027ec:	20 75 6e 
  4027ef:	48 89 45 00          	mov    %rax,0x0(%rbp)
  4027f3:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  4027f7:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  4027fe:	74 6f 20 
  402801:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
  402808:	76 65 20 
  40280b:	48 89 45 10          	mov    %rax,0x10(%rbp)
  40280f:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402813:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
  40281a:	72 20 61 
  40281d:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402821:	c7 45 28 64 64 72 65 	movl   $0x65726464,0x28(%rbp)
  402828:	66 c7 45 2c 73 73    	movw   $0x7373,0x2c(%rbp)
  40282e:	c6 45 2e 00          	movb   $0x0,0x2e(%rbp)
  402832:	89 df                	mov    %ebx,%edi
  402834:	e8 e7 e4 ff ff       	callq  400d20 <close@plt>
  402839:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  40283e:	e9 23 ff ff ff       	jmpq   402766 <submitr+0x2e7>
  402843:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
  40284a:	3a 20 55 
  40284d:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
  402854:	20 74 6f 
  402857:	48 89 45 00          	mov    %rax,0x0(%rbp)
  40285b:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  40285f:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
  402866:	65 63 74 
  402869:	48 ba 20 74 6f 20 74 	movabs $0x20656874206f7420,%rdx
  402870:	68 65 20 
  402873:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402877:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  40287b:	c7 45 20 73 65 72 76 	movl   $0x76726573,0x20(%rbp)
  402882:	66 c7 45 24 65 72    	movw   $0x7265,0x24(%rbp)
  402888:	c6 45 26 00          	movb   $0x0,0x26(%rbp)
  40288c:	89 df                	mov    %ebx,%edi
  40288e:	e8 8d e4 ff ff       	callq  400d20 <close@plt>
  402893:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402898:	e9 c9 fe ff ff       	jmpq   402766 <submitr+0x2e7>
  40289d:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
  4028a4:	3a 20 52 
  4028a7:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
  4028ae:	20 73 74 
  4028b1:	48 89 45 00          	mov    %rax,0x0(%rbp)
  4028b5:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  4028b9:	48 b8 72 69 6e 67 20 	movabs $0x6f6f7420676e6972,%rax
  4028c0:	74 6f 6f 
  4028c3:	48 ba 20 6c 61 72 67 	movabs $0x202e656772616c20,%rdx
  4028ca:	65 2e 20 
  4028cd:	48 89 45 10          	mov    %rax,0x10(%rbp)
  4028d1:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  4028d5:	48 b8 49 6e 63 72 65 	movabs $0x6573616572636e49,%rax
  4028dc:	61 73 65 
  4028df:	48 ba 20 53 55 42 4d 	movabs $0x5254494d42555320,%rdx
  4028e6:	49 54 52 
  4028e9:	48 89 45 20          	mov    %rax,0x20(%rbp)
  4028ed:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  4028f1:	48 b8 5f 4d 41 58 42 	movabs $0x46554258414d5f,%rax
  4028f8:	55 46 00 
  4028fb:	48 89 45 30          	mov    %rax,0x30(%rbp)
  4028ff:	89 df                	mov    %ebx,%edi
  402901:	e8 1a e4 ff ff       	callq  400d20 <close@plt>
  402906:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  40290b:	e9 56 fe ff ff       	jmpq   402766 <submitr+0x2e7>
  402910:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
  402917:	3a 20 52 
  40291a:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
  402921:	20 73 74 
  402924:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402928:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  40292c:	48 b8 72 69 6e 67 20 	movabs $0x6e6f6320676e6972,%rax
  402933:	63 6f 6e 
  402936:	48 ba 74 61 69 6e 73 	movabs $0x6e6120736e696174,%rdx
  40293d:	20 61 6e 
  402940:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402944:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402948:	48 b8 20 69 6c 6c 65 	movabs $0x6c6167656c6c6920,%rax
  40294f:	67 61 6c 
  402952:	48 ba 20 6f 72 20 75 	movabs $0x72706e7520726f20,%rdx
  402959:	6e 70 72 
  40295c:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402960:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402964:	48 b8 69 6e 74 61 62 	movabs $0x20656c6261746e69,%rax
  40296b:	6c 65 20 
  40296e:	48 ba 63 68 61 72 61 	movabs $0x6574636172616863,%rdx
  402975:	63 74 65 
  402978:	48 89 45 30          	mov    %rax,0x30(%rbp)
  40297c:	48 89 55 38          	mov    %rdx,0x38(%rbp)
  402980:	66 c7 45 40 72 2e    	movw   $0x2e72,0x40(%rbp)
  402986:	c6 45 42 00          	movb   $0x0,0x42(%rbp)
  40298a:	89 df                	mov    %ebx,%edi
  40298c:	e8 8f e3 ff ff       	callq  400d20 <close@plt>
  402991:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402996:	e9 cb fd ff ff       	jmpq   402766 <submitr+0x2e7>
  40299b:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  4029a2:	3a 20 43 
  4029a5:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  4029ac:	20 75 6e 
  4029af:	48 89 45 00          	mov    %rax,0x0(%rbp)
  4029b3:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  4029b7:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  4029be:	74 6f 20 
  4029c1:	48 ba 77 72 69 74 65 	movabs $0x6f74206574697277,%rdx
  4029c8:	20 74 6f 
  4029cb:	48 89 45 10          	mov    %rax,0x10(%rbp)
  4029cf:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  4029d3:	48 b8 20 74 68 65 20 	movabs $0x7365722065687420,%rax
  4029da:	72 65 73 
  4029dd:	48 ba 75 6c 74 20 73 	movabs $0x7672657320746c75,%rdx
  4029e4:	65 72 76 
  4029e7:	48 89 45 20          	mov    %rax,0x20(%rbp)
  4029eb:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  4029ef:	66 c7 45 30 65 72    	movw   $0x7265,0x30(%rbp)
  4029f5:	c6 45 32 00          	movb   $0x0,0x32(%rbp)
  4029f9:	89 df                	mov    %ebx,%edi
  4029fb:	e8 20 e3 ff ff       	callq  400d20 <close@plt>
  402a00:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402a05:	e9 5c fd ff ff       	jmpq   402766 <submitr+0x2e7>
  402a0a:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402a11:	3a 20 43 
  402a14:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402a1b:	20 75 6e 
  402a1e:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402a22:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  402a26:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402a2d:	74 6f 20 
  402a30:	48 ba 72 65 61 64 20 	movabs $0x7269662064616572,%rdx
  402a37:	66 69 72 
  402a3a:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402a3e:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402a42:	48 b8 73 74 20 68 65 	movabs $0x6564616568207473,%rax
  402a49:	61 64 65 
  402a4c:	48 ba 72 20 66 72 6f 	movabs $0x72206d6f72662072,%rdx
  402a53:	6d 20 72 
  402a56:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402a5a:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402a5e:	48 b8 65 73 75 6c 74 	movabs $0x657320746c757365,%rax
  402a65:	20 73 65 
  402a68:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402a6c:	c7 45 38 72 76 65 72 	movl   $0x72657672,0x38(%rbp)
  402a73:	c6 45 3c 00          	movb   $0x0,0x3c(%rbp)
  402a77:	89 df                	mov    %ebx,%edi
  402a79:	e8 a2 e2 ff ff       	callq  400d20 <close@plt>
  402a7e:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402a83:	e9 de fc ff ff       	jmpq   402766 <submitr+0x2e7>
  402a88:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  402a8f:	00 
  402a90:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
  402a95:	ba 00 20 00 00       	mov    $0x2000,%edx
  402a9a:	e8 29 f8 ff ff       	callq  4022c8 <rio_readlineb>
  402a9f:	48 85 c0             	test   %rax,%rax
  402aa2:	0f 8e 96 00 00 00    	jle    402b3e <submitr+0x6bf>
  402aa8:	44 8b 44 24 2c       	mov    0x2c(%rsp),%r8d
  402aad:	41 81 f8 c8 00 00 00 	cmp    $0xc8,%r8d
  402ab4:	0f 85 08 01 00 00    	jne    402bc2 <submitr+0x743>
  402aba:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  402ac1:	00 
  402ac2:	48 89 ef             	mov    %rbp,%rdi
  402ac5:	e8 e6 e1 ff ff       	callq  400cb0 <strcpy@plt>
  402aca:	89 df                	mov    %ebx,%edi
  402acc:	e8 4f e2 ff ff       	callq  400d20 <close@plt>
  402ad1:	b9 04 00 00 00       	mov    $0x4,%ecx
  402ad6:	48 8d 3d 73 0c 00 00 	lea    0xc73(%rip),%rdi        # 403750 <trans_char+0xc0>
  402add:	48 89 ee             	mov    %rbp,%rsi
  402ae0:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  402ae2:	0f 97 c0             	seta   %al
  402ae5:	1c 00                	sbb    $0x0,%al
  402ae7:	0f be c0             	movsbl %al,%eax
  402aea:	85 c0                	test   %eax,%eax
  402aec:	0f 84 74 fc ff ff    	je     402766 <submitr+0x2e7>
  402af2:	b9 05 00 00 00       	mov    $0x5,%ecx
  402af7:	48 8d 3d 56 0c 00 00 	lea    0xc56(%rip),%rdi        # 403754 <trans_char+0xc4>
  402afe:	48 89 ee             	mov    %rbp,%rsi
  402b01:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  402b03:	0f 97 c0             	seta   %al
  402b06:	1c 00                	sbb    $0x0,%al
  402b08:	0f be c0             	movsbl %al,%eax
  402b0b:	85 c0                	test   %eax,%eax
  402b0d:	0f 84 53 fc ff ff    	je     402766 <submitr+0x2e7>
  402b13:	b9 03 00 00 00       	mov    $0x3,%ecx
  402b18:	48 8d 3d 3a 0c 00 00 	lea    0xc3a(%rip),%rdi        # 403759 <trans_char+0xc9>
  402b1f:	48 89 ee             	mov    %rbp,%rsi
  402b22:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  402b24:	0f 97 c0             	seta   %al
  402b27:	1c 00                	sbb    $0x0,%al
  402b29:	0f be c0             	movsbl %al,%eax
  402b2c:	85 c0                	test   %eax,%eax
  402b2e:	0f 84 32 fc ff ff    	je     402766 <submitr+0x2e7>
  402b34:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402b39:	e9 28 fc ff ff       	jmpq   402766 <submitr+0x2e7>
  402b3e:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402b45:	3a 20 43 
  402b48:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402b4f:	20 75 6e 
  402b52:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402b56:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  402b5a:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402b61:	74 6f 20 
  402b64:	48 ba 72 65 61 64 20 	movabs $0x6174732064616572,%rdx
  402b6b:	73 74 61 
  402b6e:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402b72:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402b76:	48 b8 74 75 73 20 6d 	movabs $0x7373656d20737574,%rax
  402b7d:	65 73 73 
  402b80:	48 ba 61 67 65 20 66 	movabs $0x6d6f726620656761,%rdx
  402b87:	72 6f 6d 
  402b8a:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402b8e:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402b92:	48 b8 20 72 65 73 75 	movabs $0x20746c7573657220,%rax
  402b99:	6c 74 20 
  402b9c:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402ba0:	c7 45 38 73 65 72 76 	movl   $0x76726573,0x38(%rbp)
  402ba7:	66 c7 45 3c 65 72    	movw   $0x7265,0x3c(%rbp)
  402bad:	c6 45 3e 00          	movb   $0x0,0x3e(%rbp)
  402bb1:	89 df                	mov    %ebx,%edi
  402bb3:	e8 68 e1 ff ff       	callq  400d20 <close@plt>
  402bb8:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402bbd:	e9 a4 fb ff ff       	jmpq   402766 <submitr+0x2e7>
  402bc2:	4c 8d 8c 24 50 80 00 	lea    0x8050(%rsp),%r9
  402bc9:	00 
  402bca:	48 8d 0d 37 0b 00 00 	lea    0xb37(%rip),%rcx        # 403708 <trans_char+0x78>
  402bd1:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
  402bd8:	be 01 00 00 00       	mov    $0x1,%esi
  402bdd:	48 89 ef             	mov    %rbp,%rdi
  402be0:	b8 00 00 00 00       	mov    $0x0,%eax
  402be5:	e8 76 e2 ff ff       	callq  400e60 <__sprintf_chk@plt>
  402bea:	89 df                	mov    %ebx,%edi
  402bec:	e8 2f e1 ff ff       	callq  400d20 <close@plt>
  402bf1:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402bf6:	e9 6b fb ff ff       	jmpq   402766 <submitr+0x2e7>
  402bfb:	e8 e0 e0 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000402c00 <init_timeout>:
  402c00:	85 ff                	test   %edi,%edi
  402c02:	74 28                	je     402c2c <init_timeout+0x2c>
  402c04:	53                   	push   %rbx
  402c05:	89 fb                	mov    %edi,%ebx
  402c07:	85 ff                	test   %edi,%edi
  402c09:	78 1a                	js     402c25 <init_timeout+0x25>
  402c0b:	48 8d 35 9d f5 ff ff 	lea    -0xa63(%rip),%rsi        # 4021af <sigalrm_handler>
  402c12:	bf 0e 00 00 00       	mov    $0xe,%edi
  402c17:	e8 24 e1 ff ff       	callq  400d40 <signal@plt>
  402c1c:	89 df                	mov    %ebx,%edi
  402c1e:	e8 ed e0 ff ff       	callq  400d10 <alarm@plt>
  402c23:	5b                   	pop    %rbx
  402c24:	c3                   	retq   
  402c25:	bb 00 00 00 00       	mov    $0x0,%ebx
  402c2a:	eb df                	jmp    402c0b <init_timeout+0xb>
  402c2c:	f3 c3                	repz retq 

0000000000402c2e <init_driver>:
  402c2e:	41 54                	push   %r12
  402c30:	55                   	push   %rbp
  402c31:	53                   	push   %rbx
  402c32:	48 83 ec 20          	sub    $0x20,%rsp
  402c36:	49 89 fc             	mov    %rdi,%r12
  402c39:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402c40:	00 00 
  402c42:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  402c47:	31 c0                	xor    %eax,%eax
  402c49:	be 01 00 00 00       	mov    $0x1,%esi
  402c4e:	bf 0d 00 00 00       	mov    $0xd,%edi
  402c53:	e8 e8 e0 ff ff       	callq  400d40 <signal@plt>
  402c58:	be 01 00 00 00       	mov    $0x1,%esi
  402c5d:	bf 1d 00 00 00       	mov    $0x1d,%edi
  402c62:	e8 d9 e0 ff ff       	callq  400d40 <signal@plt>
  402c67:	be 01 00 00 00       	mov    $0x1,%esi
  402c6c:	bf 1d 00 00 00       	mov    $0x1d,%edi
  402c71:	e8 ca e0 ff ff       	callq  400d40 <signal@plt>
  402c76:	ba 00 00 00 00       	mov    $0x0,%edx
  402c7b:	be 01 00 00 00       	mov    $0x1,%esi
  402c80:	bf 02 00 00 00       	mov    $0x2,%edi
  402c85:	e8 e6 e1 ff ff       	callq  400e70 <socket@plt>
  402c8a:	85 c0                	test   %eax,%eax
  402c8c:	0f 88 a3 00 00 00    	js     402d35 <init_driver+0x107>
  402c92:	89 c3                	mov    %eax,%ebx
  402c94:	48 8d 3d c1 0a 00 00 	lea    0xac1(%rip),%rdi        # 40375c <trans_char+0xcc>
  402c9b:	e8 b0 e0 ff ff       	callq  400d50 <gethostbyname@plt>
  402ca0:	48 85 c0             	test   %rax,%rax
  402ca3:	0f 84 df 00 00 00    	je     402d88 <init_driver+0x15a>
  402ca9:	48 89 e5             	mov    %rsp,%rbp
  402cac:	48 c7 44 24 02 00 00 	movq   $0x0,0x2(%rsp)
  402cb3:	00 00 
  402cb5:	c7 45 0a 00 00 00 00 	movl   $0x0,0xa(%rbp)
  402cbc:	66 c7 45 0e 00 00    	movw   $0x0,0xe(%rbp)
  402cc2:	66 c7 04 24 02 00    	movw   $0x2,(%rsp)
  402cc8:	48 63 50 14          	movslq 0x14(%rax),%rdx
  402ccc:	48 8b 40 18          	mov    0x18(%rax),%rax
  402cd0:	48 8b 30             	mov    (%rax),%rsi
  402cd3:	48 8d 7d 04          	lea    0x4(%rbp),%rdi
  402cd7:	b9 0c 00 00 00       	mov    $0xc,%ecx
  402cdc:	e8 7f e0 ff ff       	callq  400d60 <__memmove_chk@plt>
  402ce1:	66 c7 44 24 02 3c 9a 	movw   $0x9a3c,0x2(%rsp)
  402ce8:	ba 10 00 00 00       	mov    $0x10,%edx
  402ced:	48 89 ee             	mov    %rbp,%rsi
  402cf0:	89 df                	mov    %ebx,%edi
  402cf2:	e8 49 e1 ff ff       	callq  400e40 <connect@plt>
  402cf7:	85 c0                	test   %eax,%eax
  402cf9:	0f 88 fb 00 00 00    	js     402dfa <init_driver+0x1cc>
  402cff:	89 df                	mov    %ebx,%edi
  402d01:	e8 1a e0 ff ff       	callq  400d20 <close@plt>
  402d06:	66 41 c7 04 24 4f 4b 	movw   $0x4b4f,(%r12)
  402d0d:	41 c6 44 24 02 00    	movb   $0x0,0x2(%r12)
  402d13:	b8 00 00 00 00       	mov    $0x0,%eax
  402d18:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
  402d1d:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  402d24:	00 00 
  402d26:	0f 85 28 01 00 00    	jne    402e54 <init_driver+0x226>
  402d2c:	48 83 c4 20          	add    $0x20,%rsp
  402d30:	5b                   	pop    %rbx
  402d31:	5d                   	pop    %rbp
  402d32:	41 5c                	pop    %r12
  402d34:	c3                   	retq   
  402d35:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402d3c:	3a 20 43 
  402d3f:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402d46:	20 75 6e 
  402d49:	49 89 04 24          	mov    %rax,(%r12)
  402d4d:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
  402d52:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402d59:	74 6f 20 
  402d5c:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
  402d63:	65 20 73 
  402d66:	49 89 44 24 10       	mov    %rax,0x10(%r12)
  402d6b:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
  402d70:	41 c7 44 24 20 6f 63 	movl   $0x656b636f,0x20(%r12)
  402d77:	6b 65 
  402d79:	66 41 c7 44 24 24 74 	movw   $0x74,0x24(%r12)
  402d80:	00 
  402d81:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402d86:	eb 90                	jmp    402d18 <init_driver+0xea>
  402d88:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
  402d8f:	3a 20 44 
  402d92:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
  402d99:	20 75 6e 
  402d9c:	49 89 04 24          	mov    %rax,(%r12)
  402da0:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
  402da5:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402dac:	74 6f 20 
  402daf:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
  402db6:	76 65 20 
  402db9:	49 89 44 24 10       	mov    %rax,0x10(%r12)
  402dbe:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
  402dc3:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
  402dca:	72 20 61 
  402dcd:	49 89 44 24 20       	mov    %rax,0x20(%r12)
  402dd2:	41 c7 44 24 28 64 64 	movl   $0x65726464,0x28(%r12)
  402dd9:	72 65 
  402ddb:	66 41 c7 44 24 2c 73 	movw   $0x7373,0x2c(%r12)
  402de2:	73 
  402de3:	41 c6 44 24 2e 00    	movb   $0x0,0x2e(%r12)
  402de9:	89 df                	mov    %ebx,%edi
  402deb:	e8 30 df ff ff       	callq  400d20 <close@plt>
  402df0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402df5:	e9 1e ff ff ff       	jmpq   402d18 <init_driver+0xea>
  402dfa:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
  402e01:	3a 20 55 
  402e04:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
  402e0b:	20 74 6f 
  402e0e:	49 89 04 24          	mov    %rax,(%r12)
  402e12:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
  402e17:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
  402e1e:	65 63 74 
  402e21:	48 ba 20 74 6f 20 73 	movabs $0x76726573206f7420,%rdx
  402e28:	65 72 76 
  402e2b:	49 89 44 24 10       	mov    %rax,0x10(%r12)
  402e30:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
  402e35:	66 41 c7 44 24 20 65 	movw   $0x7265,0x20(%r12)
  402e3c:	72 
  402e3d:	41 c6 44 24 22 00    	movb   $0x0,0x22(%r12)
  402e43:	89 df                	mov    %ebx,%edi
  402e45:	e8 d6 de ff ff       	callq  400d20 <close@plt>
  402e4a:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402e4f:	e9 c4 fe ff ff       	jmpq   402d18 <init_driver+0xea>
  402e54:	e8 87 de ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000402e59 <driver_post>:
  402e59:	53                   	push   %rbx
  402e5a:	4c 89 cb             	mov    %r9,%rbx
  402e5d:	45 85 c0             	test   %r8d,%r8d
  402e60:	75 18                	jne    402e7a <driver_post+0x21>
  402e62:	48 85 ff             	test   %rdi,%rdi
  402e65:	74 05                	je     402e6c <driver_post+0x13>
  402e67:	80 3f 00             	cmpb   $0x0,(%rdi)
  402e6a:	75 37                	jne    402ea3 <driver_post+0x4a>
  402e6c:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
  402e71:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
  402e75:	44 89 c0             	mov    %r8d,%eax
  402e78:	5b                   	pop    %rbx
  402e79:	c3                   	retq   
  402e7a:	48 89 ca             	mov    %rcx,%rdx
  402e7d:	48 8d 35 f0 08 00 00 	lea    0x8f0(%rip),%rsi        # 403774 <trans_char+0xe4>
  402e84:	bf 01 00 00 00       	mov    $0x1,%edi
  402e89:	b8 00 00 00 00       	mov    $0x0,%eax
  402e8e:	e8 4d df ff ff       	callq  400de0 <__printf_chk@plt>
  402e93:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
  402e98:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
  402e9c:	b8 00 00 00 00       	mov    $0x0,%eax
  402ea1:	eb d5                	jmp    402e78 <driver_post+0x1f>
  402ea3:	48 83 ec 08          	sub    $0x8,%rsp
  402ea7:	41 51                	push   %r9
  402ea9:	49 89 c9             	mov    %rcx,%r9
  402eac:	49 89 d0             	mov    %rdx,%r8
  402eaf:	48 89 f9             	mov    %rdi,%rcx
  402eb2:	48 89 f2             	mov    %rsi,%rdx
  402eb5:	be 9a 3c 00 00       	mov    $0x3c9a,%esi
  402eba:	48 8d 3d 9b 08 00 00 	lea    0x89b(%rip),%rdi        # 40375c <trans_char+0xcc>
  402ec1:	e8 b9 f5 ff ff       	callq  40247f <submitr>
  402ec6:	48 83 c4 10          	add    $0x10,%rsp
  402eca:	eb ac                	jmp    402e78 <driver_post+0x1f>

0000000000402ecc <check>:
  402ecc:	89 f8                	mov    %edi,%eax
  402ece:	c1 e8 1c             	shr    $0x1c,%eax
  402ed1:	85 c0                	test   %eax,%eax
  402ed3:	74 1d                	je     402ef2 <check+0x26>
  402ed5:	b9 00 00 00 00       	mov    $0x0,%ecx
  402eda:	83 f9 1f             	cmp    $0x1f,%ecx
  402edd:	7f 0d                	jg     402eec <check+0x20>
  402edf:	89 f8                	mov    %edi,%eax
  402ee1:	d3 e8                	shr    %cl,%eax
  402ee3:	3c 0a                	cmp    $0xa,%al
  402ee5:	74 11                	je     402ef8 <check+0x2c>
  402ee7:	83 c1 08             	add    $0x8,%ecx
  402eea:	eb ee                	jmp    402eda <check+0xe>
  402eec:	b8 01 00 00 00       	mov    $0x1,%eax
  402ef1:	c3                   	retq   
  402ef2:	b8 00 00 00 00       	mov    $0x0,%eax
  402ef7:	c3                   	retq   
  402ef8:	b8 00 00 00 00       	mov    $0x0,%eax
  402efd:	c3                   	retq   

0000000000402efe <gencookie>:
  402efe:	53                   	push   %rbx
  402eff:	83 c7 01             	add    $0x1,%edi
  402f02:	e8 89 dd ff ff       	callq  400c90 <srandom@plt>
  402f07:	e8 94 de ff ff       	callq  400da0 <random@plt>
  402f0c:	89 c3                	mov    %eax,%ebx
  402f0e:	89 c7                	mov    %eax,%edi
  402f10:	e8 b7 ff ff ff       	callq  402ecc <check>
  402f15:	85 c0                	test   %eax,%eax
  402f17:	74 ee                	je     402f07 <gencookie+0x9>
  402f19:	89 d8                	mov    %ebx,%eax
  402f1b:	5b                   	pop    %rbx
  402f1c:	c3                   	retq   
  402f1d:	0f 1f 00             	nopl   (%rax)

0000000000402f20 <__libc_csu_init>:
  402f20:	41 57                	push   %r15
  402f22:	41 56                	push   %r14
  402f24:	49 89 d7             	mov    %rdx,%r15
  402f27:	41 55                	push   %r13
  402f29:	41 54                	push   %r12
  402f2b:	4c 8d 25 de 1e 20 00 	lea    0x201ede(%rip),%r12        # 604e10 <__frame_dummy_init_array_entry>
  402f32:	55                   	push   %rbp
  402f33:	48 8d 2d de 1e 20 00 	lea    0x201ede(%rip),%rbp        # 604e18 <__init_array_end>
  402f3a:	53                   	push   %rbx
  402f3b:	41 89 fd             	mov    %edi,%r13d
  402f3e:	49 89 f6             	mov    %rsi,%r14
  402f41:	4c 29 e5             	sub    %r12,%rbp
  402f44:	48 83 ec 08          	sub    $0x8,%rsp
  402f48:	48 c1 fd 03          	sar    $0x3,%rbp
  402f4c:	e8 f7 dc ff ff       	callq  400c48 <_init>
  402f51:	48 85 ed             	test   %rbp,%rbp
  402f54:	74 20                	je     402f76 <__libc_csu_init+0x56>
  402f56:	31 db                	xor    %ebx,%ebx
  402f58:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  402f5f:	00 
  402f60:	4c 89 fa             	mov    %r15,%rdx
  402f63:	4c 89 f6             	mov    %r14,%rsi
  402f66:	44 89 ef             	mov    %r13d,%edi
  402f69:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  402f6d:	48 83 c3 01          	add    $0x1,%rbx
  402f71:	48 39 dd             	cmp    %rbx,%rbp
  402f74:	75 ea                	jne    402f60 <__libc_csu_init+0x40>
  402f76:	48 83 c4 08          	add    $0x8,%rsp
  402f7a:	5b                   	pop    %rbx
  402f7b:	5d                   	pop    %rbp
  402f7c:	41 5c                	pop    %r12
  402f7e:	41 5d                	pop    %r13
  402f80:	41 5e                	pop    %r14
  402f82:	41 5f                	pop    %r15
  402f84:	c3                   	retq   
  402f85:	90                   	nop
  402f86:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  402f8d:	00 00 00 

0000000000402f90 <__libc_csu_fini>:
  402f90:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000402f94 <_fini>:
  402f94:	48 83 ec 08          	sub    $0x8,%rsp
  402f98:	48 83 c4 08          	add    $0x8,%rsp
  402f9c:	c3                   	retq   
