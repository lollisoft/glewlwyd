!function(e,t){if("object"==typeof exports&&"object"==typeof module)module.exports=t();else if("function"==typeof define&&define.amd)define([],t);else{var n=t();for(var r in n)("object"==typeof exports?exports:e)[r]=n[r]}}(window,(function(){return function(e){function t(t){for(var r,i,c=t[0],l=t[1],u=t[2],f=0,p=[];f<c.length;f++)i=c[f],Object.prototype.hasOwnProperty.call(o,i)&&o[i]&&p.push(o[i][0]),o[i]=0;for(r in l)Object.prototype.hasOwnProperty.call(l,r)&&(e[r]=l[r]);for(s&&s(t);p.length;)p.shift()();return a.push.apply(a,u||[]),n()}function n(){for(var e,t=0;t<a.length;t++){for(var n=a[t],r=!0,c=1;c<n.length;c++){var l=n[c];0!==o[l]&&(r=!1)}r&&(a.splice(t--,1),e=i(i.s=n[0]))}return e}var r={},o={2:0},a=[];function i(t){if(r[t])return r[t].exports;var n=r[t]={i:t,l:!1,exports:{}};return e[t].call(n.exports,n,n.exports,i),n.l=!0,n.exports}i.m=e,i.c=r,i.d=function(e,t,n){i.o(e,t)||Object.defineProperty(e,t,{enumerable:!0,get:n})},i.r=function(e){"undefined"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(e,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(e,"__esModule",{value:!0})},i.t=function(e,t){if(1&t&&(e=i(e)),8&t)return e;if(4&t&&"object"==typeof e&&e&&e.__esModule)return e;var n=Object.create(null);if(i.r(n),Object.defineProperty(n,"default",{enumerable:!0,value:e}),2&t&&"string"!=typeof e)for(var r in e)i.d(n,r,function(t){return e[t]}.bind(null,r));return n},i.n=function(e){var t=e&&e.__esModule?function(){return e.default}:function(){return e};return i.d(t,"a",t),t},i.o=function(e,t){return Object.prototype.hasOwnProperty.call(e,t)},i.p="";var c=window.webpackJsonp=window.webpackJsonp||[],l=c.push.bind(c);c.push=t,c=c.slice();for(var u=0;u<c.length;u++)t(c[u]);var s=l;return a.push([27,0]),n()}({2:function(e,t,n){"use strict";function r(e,t){for(var n=0;n<t.length;n++){var r=t[n];r.enumerable=r.enumerable||!1,r.configurable=!0,"value"in r&&(r.writable=!0),Object.defineProperty(e,r.key,r)}}var o=new(function(){function e(){!function(e,t){if(!(e instanceof t))throw new TypeError("Cannot call a class as a function")}(this,e),this.subscriberList={}}return function(e,t,n){t&&r(e.prototype,t),Object.defineProperty(e,"prototype",{writable:!1})}(e,[{key:"subscribe",value:function(e,t){this.subscriberList[e]=t}},{key:"sendMessage",value:function(e,t){if(Array.isArray(e)||(e=[e]),"broadcast"===e[0])this.subscriberList.forEach((function(e,n){e({type:"broadcast",message:t})}));else for(var n in this.subscriberList)e.indexOf(n)>=0&&this.subscriberList[n](t)}}]),e}());t.a=o},27:function(e,t,n){"use strict";n.r(t);var r=n(0),o=n.n(r),a=n(5),i=n.n(a),c=n(1),l=n(10),u=n(11),s=n(3);function f(e){return(f="function"==typeof Symbol&&"symbol"==typeof Symbol.iterator?function(e){return typeof e}:function(e){return e&&"function"==typeof Symbol&&e.constructor===Symbol&&e!==Symbol.prototype?"symbol":typeof e})(e)}function p(e,t){for(var n=0;n<t.length;n++){var r=t[n];r.enumerable=r.enumerable||!1,r.configurable=!0,"value"in r&&(r.writable=!0),Object.defineProperty(e,r.key,r)}}function d(e,t){return(d=Object.setPrototypeOf||function(e,t){return e.__proto__=t,e})(e,t)}function y(e){if(void 0===e)throw new ReferenceError("this hasn't been initialised - super() hasn't been called");return e}function b(e){return(b=Object.setPrototypeOf?Object.getPrototypeOf:function(e){return e.__proto__||Object.getPrototypeOf(e)})(e)}var h=function(e){!function(e,t){if("function"!=typeof t&&null!==t)throw new TypeError("Super expression must either be null or a function");e.prototype=Object.create(t&&t.prototype,{constructor:{value:e,writable:!0,configurable:!0}}),Object.defineProperty(e,"prototype",{writable:!1}),t&&d(e,t)}(n,r.Component);var t=function(e){var t=function(){if("undefined"==typeof Reflect||!Reflect.construct)return!1;if(Reflect.construct.sham)return!1;if("function"==typeof Proxy)return!0;try{return Boolean.prototype.valueOf.call(Reflect.construct(Boolean,[],(function(){}))),!0}catch(e){return!1}}();return function(){var n,r=b(e);if(t){var o=b(this).constructor;n=Reflect.construct(r,arguments,o)}else n=r.apply(this,arguments);return function(e,t){if(t&&("object"===f(t)||"function"==typeof t))return t;if(void 0!==t)throw new TypeError("Derived constructors may only return object or undefined");return y(e)}(this,n)}}(n);function n(e){var r;return function(e,t){if(!(e instanceof t))throw new TypeError("Cannot call a class as a function")}(this,n),(r=t.call(this,e)).state={config:e.config,errorAuthentication:!1,unknownError:r.parseState(e.config.params.state,e.config),gotoProfile:!1,gotoLogin:!1},r.gotoLogin=r.gotoLogin.bind(y(r)),r.gotoProfile=r.gotoProfile.bind(y(r)),r}return function(e,t,n){t&&p(e.prototype,t),Object.defineProperty(e,"prototype",{writable:!1})}(n,[{key:"Base64DecodeUrl",value:function(e){return e.length%4==2?e+="==":e.length%4==3&&(e+="="),e.replace(/-/g,"+").replace(/_/g,"/")}},{key:"parseState",value:function(e,t){var n=this,r=!1;if(e){var o=!1,a=!1;try{o=JSON.parse(atob(this.Base64DecodeUrl(e)))}catch(e){a=!0}if(a)r=!0;else if("registration"===o.type){var i={scheme_name:o.module,scheme_type:"oauth2",username:o.username,value:{action:"callback",provider:o.provider,state:e,redirect_to:window.location.href}};$.ajax({method:"POST",url:o.register_url+"/profile/scheme/register/",data:JSON.stringify(i),contentType:"application/json; charset=utf-8"}).then((function(){n.setState({stateDecoded:o},(function(){var e=o.complete_url;e.indexOf("?")>-1?e+="&":e+="?",e+="scheme_name="+o.module+"&provider="+o.provider,window.location.href=e}))})).fail((function(e){401===e.status?n.setState({stateDecoded:o,errorAuthentication:!0,gotoProfile:!0}):n.setState({stateDecoded:o,unknownError:!0,gotoProfile:!0})}))}else"authentication"===o.type?(i={scheme_name:o.module,scheme_type:"oauth2",value:{provider:o.provider,state:e,redirect_to:window.location.href}},o.username?i.username=o.username:i.value.action="verify",$.ajax({method:"POST",url:t.GlewlwydUrl+"/"+t.api_prefix+"/auth/",data:JSON.stringify(i),contentType:"application/json; charset=utf-8"}).then((function(){n.setState({stateDecoded:o},(function(){window.location.href=o.callback_url}))})).fail((function(e){401===e.status?n.setState({stateDecoded:o,errorAuthentication:!0,gotoLogin:!0}):n.setState({stateDecoded:o,unknownError:!0,gotoLogin:!0})}))):r=!0}else r=!0;return r}},{key:"gotoLogin",value:function(){window.location.href=this.state.stateDecoded.callback_url}},{key:"gotoProfile",value:function(){if(this.state.stateDecoded&&this.state.stateDecoded.complete_url){var e=this.state.stateDecoded.complete_url;e.indexOf("?")>-1?e+="&":e+="?",e+="scheme_name="+this.state.stateDecoded.module+"&provider="+this.state.stateDecoded.provider,window.location.href=e}else window.location.href=this.state.config.ProfileUrl}},{key:"render",value:function(){var e;return this.state.config?this.state.errorAuthentication||this.state.unknownError?(e=this.state.gotoLogin?o.a.createElement("button",{type:"button",className:"btn btn-primary",id:"buttonBack",onClick:this.gotoLogin},c.a.t("callback.button-login")):o.a.createElement("button",{type:"button",className:"btn btn-primary",id:"buttonBack",onClick:this.gotoProfile},c.a.t("callback.button-profile")),o.a.createElement("div",{className:"perfect-centering"},o.a.createElement("div",{className:"alert alert-danger"},o.a.createElement("h3",null,this.state.errorAuthentication?c.a.t("callback.authentication-error"):c.a.t("callback.unknown-error"))),o.a.createElement("div",{className:"row justify-content-md-center"},e))):o.a.createElement("div",{className:"perfect-centering"},o.a.createElement("div",{className:"alert alert-info"},o.a.createElement("h3",null,c.a.t("callback.authentication-success")))):o.a.createElement("div",{"aria-live":"polite","aria-atomic":"true",className:"glwd-container"},o.a.createElement("div",{className:"card center glwd-card",id:"userCard",tabIndex:"-1",role:"dialog"},o.a.createElement("div",{className:"card-header"},o.a.createElement("h4",null,o.a.createElement("span",{className:"badge badge-danger"},c.a.t("error-api-connect"))))))}}]),n}(),m=n(8);function g(e,t){(null==t||t>e.length)&&(t=e.length);for(var n=0,r=new Array(t);n<t;n++)r[n]=e[n];return r}function v(e){if(e){var t={};return e.split("&").map((function(e){var n=function(e,t){return function(e){if(Array.isArray(e))return e}(e)||function(e,t){var n=null==e?null:"undefined"!=typeof Symbol&&e[Symbol.iterator]||e["@@iterator"];if(null!=n){var r,o,a=[],i=!0,c=!1;try{for(n=n.call(e);!(i=(r=n.next()).done)&&(a.push(r.value),!t||a.length!==t);i=!0);}catch(e){c=!0,o=e}finally{try{i||null==n.return||n.return()}finally{if(c)throw o}}return a}}(e,t)||function(e,t){if(e){if("string"==typeof e)return g(e,t);var n=Object.prototype.toString.call(e).slice(8,-1);return"Object"===n&&e.constructor&&(n=e.constructor.name),"Map"===n||"Set"===n?Array.from(e):"Arguments"===n||/^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(n)?g(e,t):void 0}}(e,t)||function(){throw new TypeError("Invalid attempt to destructure non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method.")}()}(e.split("="),2),r=n[0],o=n[1];t[r]=decodeURIComponent(o)})),t}return{}}var w={fallbackLng:"en",ns:["translations"],defaultNS:"translations",backend:{loadPath:"locales/{{lng}}/{{ns}}.json"}};v(window.location.href.split("?")[1]).ui_locales&&(w.lng=v(window.location.href.split("?")[1]).ui_locales.split(" ")[0]);try{c.a.use(l.a).use(u.a).init(w).then((function(){s.a.request("config.json").then((function(e){e.lang||(e.lang=["en","fr","nl","de"]),s.a.request(e.GlewlwydUrl+"config/").then((function(t){s.a.setConfig(e.GlewlwydUrl+t.api_prefix);var n=Object.assign({params:{state:v(window.location.href.split("?")[1]).state||v(window.location.hash.substring(1)).state||!1}},e,t);i.a.render(o.a.createElement(h,{config:n}),document.getElementById("root"))})).fail((function(e){i.a.render(o.a.createElement(h,{config:!1}),document.getElementById("root"))}))})).fail((function(e){i.a.render(o.a.createElement(m.a,null),document.getElementById("root"))}))}))}catch(e){$("#root").html('<div class="alert alert-danger" role="alert"><i class="fas fa-exclamation-triangle"></i><span class="btn-icon-right">You must use a browser compatible with Glewlwyd SSO</span></div>')}},3:function(e,t,n){"use strict";var r=n(2);function o(e,t){for(var n=0;n<t.length;n++){var r=t[n];r.enumerable=r.enumerable||!1,r.configurable=!0,"value"in r&&(r.writable=!0),Object.defineProperty(e,r.key,r)}}var a=new(function(){function e(){!function(e,t){if(!(e instanceof t))throw new TypeError("Cannot call a class as a function")}(this,e),this.GlewlwydApiPrefix="",this.GlewlwydApiPrefixSub=""}return function(e,t,n){t&&o(e.prototype,t),Object.defineProperty(e,"prototype",{writable:!1})}(e,[{key:"setConfig",value:function(e){this.GlewlwydApiPrefix=e}},{key:"getConfig",value:function(e){return this.GlewlwydApiPrefix}},{key:"setConfigSub",value:function(e){this.GlewlwydApiPrefixSub=e}},{key:"getConfigSub",value:function(e){return this.GlewlwydApiPrefixSub}},{key:"request",value:function(e){var t=arguments.length>1&&void 0!==arguments[1]?arguments[1]:"GET",n=arguments.length>2&&void 0!==arguments[2]&&arguments[2];return n&&"GET"!==t?$.ajax({method:t,url:e,data:JSON.stringify(n),contentType:"application/json; charset=utf-8"}):$.ajax({method:t,url:e})}},{key:"glewlwydRequest",value:function(e){var t=arguments.length>1&&void 0!==arguments[1]?arguments[1]:"GET",n=arguments.length>2&&void 0!==arguments[2]&&arguments[2],o=arguments.length>3&&void 0!==arguments[3]&&arguments[3];return this.request(this.GlewlwydApiPrefix+e,t,n).fail((function(e){o&&401===e.status&&r.a.sendMessage("App",{type:"loggedIn",loggedIn:!1})}))}},{key:"glewlwydRequestSub",value:function(e){var t=arguments.length>1&&void 0!==arguments[1]?arguments[1]:"GET",n=arguments.length>2&&void 0!==arguments[2]&&arguments[2];return this.request(this.GlewlwydApiPrefixSub+e,t,n)}}]),e}());t.a=a},8:function(e,t,n){"use strict";var r=n(0),o=n.n(r),a=n(1);function i(e){return(i="function"==typeof Symbol&&"symbol"==typeof Symbol.iterator?function(e){return typeof e}:function(e){return e&&"function"==typeof Symbol&&e.constructor===Symbol&&e!==Symbol.prototype?"symbol":typeof e})(e)}function c(e,t){for(var n=0;n<t.length;n++){var r=t[n];r.enumerable=r.enumerable||!1,r.configurable=!0,"value"in r&&(r.writable=!0),Object.defineProperty(e,r.key,r)}}function l(e,t){return(l=Object.setPrototypeOf||function(e,t){return e.__proto__=t,e})(e,t)}function u(e){return(u=Object.setPrototypeOf?Object.getPrototypeOf:function(e){return e.__proto__||Object.getPrototypeOf(e)})(e)}var s=function(e){!function(e,t){if("function"!=typeof t&&null!==t)throw new TypeError("Super expression must either be null or a function");e.prototype=Object.create(t&&t.prototype,{constructor:{value:e,writable:!0,configurable:!0}}),Object.defineProperty(e,"prototype",{writable:!1}),t&&l(e,t)}(n,r.Component);var t=function(e){var t=function(){if("undefined"==typeof Reflect||!Reflect.construct)return!1;if(Reflect.construct.sham)return!1;if("function"==typeof Proxy)return!0;try{return Boolean.prototype.valueOf.call(Reflect.construct(Boolean,[],(function(){}))),!0}catch(e){return!1}}();return function(){var n,r=u(e);if(t){var o=u(this).constructor;n=Reflect.construct(r,arguments,o)}else n=r.apply(this,arguments);return function(e,t){if(t&&("object"===i(t)||"function"==typeof t))return t;if(void 0!==t)throw new TypeError("Derived constructors may only return object or undefined");return function(e){if(void 0===e)throw new ReferenceError("this hasn't been initialised - super() hasn't been called");return e}(e)}(this,n)}}(n);function n(e){return function(e,t){if(!(e instanceof t))throw new TypeError("Cannot call a class as a function")}(this,n),t.call(this,e)}return function(e,t,n){t&&c(e.prototype,t),Object.defineProperty(e,"prototype",{writable:!1})}(n,[{key:"render",value:function(){return o.a.createElement("div",{className:"alert alert-danger perfect-centering",role:"alert"},a.a.t("error-config"))}}]),n}();t.a=s}})}));